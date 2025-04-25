const fps = 60;
const CANVAS_WIDTH = 700;
const CANVAS_HEIGHT = 600;

const root = document.createElement("div");
root.id = "root";
document.body.appendChild(root);

const canvas = document.createElement("canvas");
canvas.width = CANVAS_WIDTH;
canvas.height = CANVAS_HEIGHT;
root.appendChild(canvas);

let xCoordinate = -1;
canvas.addEventListener("click", (event) => {
    const rect = canvas.getBoundingClientRect();
    xCoordinate = event.clientX - rect.left;
});

const ctx = canvas.getContext("2d");

const MEMORY_SIZE: number = 8192;

let w: WebAssembly.WebAssemblyInstantiatedSource | null = null;

type InitFunction = (memory: number, size: number) => void;
type TickFunction = (deltaTime: number) => void;

WebAssembly.instantiateStreaming(fetch("connect4.wasm"), {
    env: {
        memory: new WebAssembly.Memory({ initial: MEMORY_SIZE }),
        js_width: (): number => {
            return CANVAS_WIDTH;
        },
        js_height: (): number => {
            return CANVAS_HEIGHT;
        },
        js_random: (a: number, b: number): number => {
            return Math.random() * (b - a) + a;
        },
        js_clear_canvas: (): void => {
            ctx!.clearRect(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);
        },
        js_draw_rect: (
            x: number,
            y: number,
            w: number,
            h: number,
            color: number
        ): void => {
            ctx!.fillStyle = `#${color.toString(16).padStart(6, "0")}`;
            ctx!.fillRect(x, y, w, h);
        },
        js_draw_circle: (
            x: number,
            y: number,
            r: number,
            color: number
        ): void => {
            ctx!.fillStyle = `#${color.toString(16).padStart(6, "0")}`;
            ctx!.beginPath();
            ctx!.arc(x, y, r, 0, Math.PI * 2);
            ctx!.fill();
        },
        js_log_message: (message: number, length: number): void => {
            const memory = new Uint8Array(
                (w!.instance.exports.memory as WebAssembly.Memory).buffer
            );

            const msg = new TextDecoder("utf-8").decode(
                memory.slice(message, message + length)
            );
            console.log(msg);
        },
        js_canvas_clicked_x: (): number => {
            if (xCoordinate !== -1) {
                const x = xCoordinate;
                xCoordinate = -1;
                return x;
            }
            return -1;
        },
    },
})
    .then((value) => {
        let fpsInterval = 1000 / fps;

        w = value;

        let init = w.instance.exports.init as InitFunction;
        let tick = w.instance.exports.tick as TickFunction;

        const memory = (w.instance.exports.__heap_base as WebAssembly.Global)
            .value;

        init(memory, MEMORY_SIZE);

        let then = Date.now();

        function gameLoop() {
            requestAnimationFrame(gameLoop);

            let now = Date.now();
            let elapsed = now - then;

            if (elapsed > fpsInterval) {
                then = now - (elapsed % fpsInterval);

                tick(elapsed);
            }
        }

        requestAnimationFrame(gameLoop);
    })
    .catch((e) => {
        console.error("Error instantiating the WebAssembly module", e);
    });
