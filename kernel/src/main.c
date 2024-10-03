#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

// Set the base revision to 2, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void *memcpy(void *dest, const void *src, size_t n) {
	uint8_t *pdest = (uint8_t *)dest;
	const uint8_t *psrc = (const uint8_t *)src;

	for (size_t i = 0; i < n; i++) {
		pdest[i] = psrc[i];
	}

	return dest;
}

void *memset(void *s, int c, size_t n) {
	uint8_t *p = (uint8_t *)s;

	for (size_t i = 0; i < n; i++) {
		p[i] = (uint8_t)c;
	}

	return s;
}

void *memmove(void *dest, const void *src, size_t n) {
	uint8_t *pdest = (uint8_t *)dest;
	const uint8_t *psrc = (const uint8_t *)src;

	if (src > dest) {
		for (size_t i = 0; i < n; i++) {
			pdest[i] = psrc[i];
		}
	} else if (src < dest) {
		for (size_t i = n; i > 0; i--) {
			pdest[i-1] = psrc[i-1];
		}
	}

	return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
	const uint8_t *p1 = (const uint8_t *)s1;
	const uint8_t *p2 = (const uint8_t *)s2;

	for (size_t i = 0; i < n; i++) {
		if (p1[i] != p2[i]) {
			return p1[i] < p2[i] ? -1 : 1;
		}
	}

	return 0;
}

// Halt and catch fire function.
static void hcf(void) {
	for (;;) {
#if defined (__x86_64__)
		asm ("hlt");
#elif defined (__aarch64__) || defined (__riscv)
		asm ("wfi");
#elif defined (__loongarch64)
		asm ("idle 0");
#endif
    }
}

volatile uint32_t *fb_ptr;
unsigned char *frame_buffer;
int offset = 0;
int X = 1024;
int Y = 768;
int BPP = 32;
int A = 3; // Anti-alias value, 1 to disable

int J = 0;
int K = -10;
int L = -7;
int M = 1296;
int N = 36;
int O = 255;
int P = 9;
int _ = 1 << 15;
int E, S, C, D, p, Z, W, Q, T, U, u, v, w, R, G, B;

int F(int b);
int I(int x, int Y, int X);
int H(int x);
int q(int c, int x, int y, int z, int k, int l, int m, int a, int b);
int o(int c, int x, int y, int z, int k, int l, int m, int a);
int n(int e, int f, int g, int h, int i, int j, int d, int a, int b, int V);
int t(int x, int y, int a, int b);
int r(int x, int y);


// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
	// Ensure the bootloader actually understands our base revision (see spec).
	if (LIMINE_BASE_REVISION_SUPPORTED == false) {
		hcf();
	}

	// Ensure we got a framebuffer.
	if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
		hcf();
	}

	// Fetch the first framebuffer.
	struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

	// Note: we assume the framebuffer model is RGB with 32-bit pixels.
 
	X = framebuffer->width;
	Y = framebuffer->height;
 
 /*
	while (1)
	{
		for (size_t i = 0; i < X * Y; i++) {
			volatile uint32_t *fb_ptr = framebuffer->address;
			fb_ptr[i] = 0x00ffffff; //  * (framebuffer->pitch / 4) + i
		}
		for (size_t i = 0; i < X * Y; i++) {
			volatile uint32_t *fb_ptr = framebuffer->address;
			fb_ptr[i] = 0x00000000; //  * (framebuffer->pitch / 4) + i
		}
	}
*/

	volatile uint32_t *fb_ptr = framebuffer->address;
	frame_buffer = (unsigned char *)fb_ptr;
	int x, y;
	//frame_buffer = (unsigned char *)(*(uint64_t *)(0x5080));
	//X = 1024;
	//Y = ;
	for (y=0; y<Y; y++)
		for (x=0; x<X; x++)
			r(x, y); // render each pixel

	// We're done, just hang...
	hcf();
}

int F(int b) {
	E = "1111886:6:??AAFFHHMMOO55557799@@>>>BBBGGIIKK" [b] - 64;
	C = "C@=::C@@==@=:C@=:C@=:C531/513/5131/31/531/53" [b] - 64;
	S = b < 22 ? 9 : 0;
	D = 2;
}

int I(int x, int Y, int X) {
	Y ? (X ^= Y, X * X > x ? (X ^= Y) : 0, I(x, Y / 2, X)) : (E = X);
}

int H(int x) {
	I(x, _, 0);
}

int q(int c, int x, int y, int z, int k, int l, int m, int a, int b) {
	F(c);
	x -= E * M;
	y -= S * M;
	z -= C * M;
	b = x * x / M + y * y / M + z * z / M - D * D * M;
	a = -x * k / M - y * l / M - z * m / M;
	p = ((b = a * a / M - b) >= 0 ? (I(b * M, _, 0), b = E, a + (a > b ? -b : b)) : -1.0);
}

int o(int c, int x, int y, int z, int k, int l, int m, int a) {
	Z = !c ? -1 : Z;
	c < 44 ? (q(c, x, y, z, k, l, m, 0, 0), (p > 0 && c != a && (p < W || Z < 0)) ? (W = p, Z = c) : 0, o(c + 1, x, y, z, k, l, m, a)) : 0;
}

int n(int e, int f, int g, int h, int i, int j, int d, int a, int b, int V) {
	o(0, e, f, g, h, i, j, a);
	d > 0 && Z >= 0 ? (e += h * W / M, f += i * W / M, g += j * W / M, F(Z), u = e - E * M, v = f - S * M, w = g - C * M, b = (-2 * u - 2 * v + w) / 3, H(u * u + v * v + w * w), b /= D, b *= b, b *= 200, b /= (M * M), V = Z, E != 0 ? (u = -u * M / E, v = -v * M / E, w = -w * M / E) : 0, E = (h * u + i * v + j * w) / M, h -= u * E / (M / 2), i -= v * E / (M / 2), j -= w * E / (M / 2), n(e, f, g, h, i, j, d - 1, Z, 0, 0), Q /= 2, T /= 2, U /= 2, V = V < 22 ? 7 : (V < 30 ? 1 : (V < 38 ? 2 : (V < 44 ? 4 : (V == 44 ? 6 : 3)))), Q += V & 1 ? b : 0, T += V & 2 ? b : 0, U += V & 4 ? b : 0) : (d == P ? (g += 2, j = g > 0 ? g / 8 : g / 20) : 0, j > 0 ? (U = j * j / M, Q = 255 - 250 * U / M, T = 255 - 150 * U / M, U = 255 - 100 * U / M) : (U = j * j / M, U < M / 5 ? (Q = 255 - 210 * U / M, T = 255 - 435 * U / M, U = 255 - 720 * U / M) : (U -= M / 5, Q = 213 - 110 * U / M, T = 168 - 113 * U / M, U = 111 - 85 * U / M)), d != P ? (Q /= 2, T /= 2, U /= 2) : 0);
	Q = Q < 0 ? 0 : Q > O ? O : Q;
	T = T < 0 ? 0 : T > O ? O : T;
	U = U < 0 ? 0 : U > O ? O : U;
}

int t(int x, int y, int a, int b) {
	n(M * J + M * 40 * (A * x + a) / X / A - M * 20, M * K, M * L - M * 30 * (A * y + b) / Y / A + M * 15, 0, M, 0, P, -1, 0, 0);
	R += Q;
	G += T;
	B += U;
	++a < A ? t(x, y, a, b) : (++b < A ? t(x, y, 0, b) : 0);
}

int r(int x, int y) {
	R = G = B = 0;
	t(x, y, 0, 0);
	frame_buffer[offset++] = B / A / A; // dump the pixel values straight to video RAM
	frame_buffer[offset++] = G / A / A;
	frame_buffer[offset++] = R / A / A;
	offset++;
}
