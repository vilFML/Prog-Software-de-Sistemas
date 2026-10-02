// _DEFAULT_SOURCE se necesita para strdup
#define _DEFAULT_SOURCE 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "test-util.h"
#include "desescapar.h"

// Tamano real del bloque que entrego malloc.  La funcion existe en todas
// partes pero cambia de nombre segun la plataforma; donde no hay ninguna
// se omite el test de uso de memoria.

#if defined(__APPLE__)
#include <malloc/malloc.h>
#define TAM_BLOQUE(p) malloc_size(p)
#elif defined(__GLIBC__) || defined(__linux__)
#include <malloc.h>
#define TAM_BLOQUE(p) malloc_usable_size(p)
#endif

// ----------------------------------------------------------------------

typedef struct {
  const char *in;
  const char *ref;
  int reflen;
} Caso;

#define CASO(in, ref) { in, ref, (int)sizeof(ref) - 1 }

static const Caso casos[] = {
  // ---- a. Sanidad: sin escapes, escape en los extremos, resultado vacio
  CASO("", ""),
  CASO("a", "a"),
  CASO("hola mundo", "hola mundo"),
  CASO("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789", "abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789"),
  CASO("\\nabc", "\nabc"),
  CASO("abc\\n", "abc\n"),
  CASO("\\n\\t\\\\\\\"", "\n\t\\\""),
  CASO("\\", ""),

  // ---- b. Los cinco escapes de la tabla del enunciado
  CASO("\\n", "\n"),
  CASO("\\t", "\t"),
  CASO("\\\\", "\\"),
  CASO("\\\"", "\""),
  CASO("\\x41", "A"),
  CASO("a\\nb", "a\nb"),
  CASO("\\n\\n", "\n\n"),
  CASO("\\x41\\x42", "AB"),
  CASO("x\\ty", "x\ty"),
  CASO("\\\"hola\\\"", "\"hola\""),

  // ---- c. \x consume exactamente dos digitos hexadecimales, ni mas ni menos
  CASO("\\x0a", "\n"),
  CASO("\\x0A", "\n"),
  CASO("\\xaB", "\xab"),
  CASO("\\xAb", "\xab"),
  CASO("\\x00", "\x00"),
  CASO("\\x7f", "\x7f"),
  CASO("\\x80", "\x80"),
  CASO("\\xff", "\xff"),
  CASO("\\x4142", "A42"),
  CASO("\\x414", "A4"),
  CASO("\\x41x41", "Ax41"),
  CASO("\\x41g", "Ag"),
  CASO("\\x00", "\x00"),
  CASO("\\x00", "\x00"),
  CASO("\\x10", "\x10"),
  CASO("\\x01", "\x01"),
  CASO("\\x20", " "),
  CASO("\\x02", "\x02"),
  CASO("\\x30", "0"),
  CASO("\\x03", "\x03"),
  CASO("\\x40", "@"),
  CASO("\\x04", "\x04"),
  CASO("\\x50", "P"),
  CASO("\\x05", "\x05"),
  CASO("\\x60", "`"),
  CASO("\\x06", "\x06"),
  CASO("\\x70", "p"),
  CASO("\\x07", "\x07"),
  CASO("\\x80", "\x80"),
  CASO("\\x08", "\x08"),
  CASO("\\x90", "\x90"),
  CASO("\\x09", "\t"),
  CASO("\\xa0", "\xa0"),
  CASO("\\x0a", "\n"),
  CASO("\\xb0", "\xb0"),
  CASO("\\x0b", "\x0b"),
  CASO("\\xc0", "\xc0"),
  CASO("\\x0c", "\x0c"),
  CASO("\\xd0", "\xd0"),
  CASO("\\x0d", "\x0d"),
  CASO("\\xe0", "\xe0"),
  CASO("\\x0e", "\x0e"),
  CASO("\\xf0", "\xf0"),
  CASO("\\x0f", "\x0f"),
  CASO("\\xA0", "\xa0"),
  CASO("\\x0A", "\n"),
  CASO("\\xB0", "\xb0"),
  CASO("\\x0B", "\x0b"),
  CASO("\\xC0", "\xc0"),
  CASO("\\x0C", "\x0c"),
  CASO("\\xD0", "\xd0"),
  CASO("\\x0D", "\x0d"),
  CASO("\\xE0", "\xe0"),
  CASO("\\x0E", "\x0e"),
  CASO("\\xF0", "\xf0"),
  CASO("\\x0F", "\x0f"),

  // ---- d. Bytes altos y char con signo
  CASO("\\xfe", "\xfe"),
  CASO("\\x81\\xfe\\xff", "\x81\xfe\xff"),
  CASO("\xff", "\xff"),
  CASO("a\xff" "b", "a\xff" "b"),
  CASO("\\x\xff", "x\xff"),
  CASO("\\x\xff\xff", "x\xff\xff"),
  CASO("\\\xff", "\xff"),
  CASO("\\x7\xff", "x7\xff"),

  // ---- e. Escapes de C que NO estan en la tabla: se elimina solo la barra
  CASO("\\0", "0"),
  CASO("a\\0b", "a0b"),
  CASO("\\r", "r"),
  CASO("\\a", "a"),
  CASO("\\b", "b"),
  CASO("\\f", "f"),
  CASO("\\v", "v"),
  CASO("\\'", "'"),
  CASO("\\?", "?"),
  CASO("\\e", "e"),
  CASO("\\X41", "X41"),
  CASO("\\N", "N"),
  CASO("\\T", "T"),
  CASO("\\1", "1"),
  CASO("\\7", "7"),
  CASO("\\ ", " "),
  CASO("\\q", "q"),

  // ---- f. \x mal formado: se elimina solo la barra, la x se mantiene
  CASO("\\xg1", "xg1"),
  CASO("\\x1g", "x1g"),
  CASO("\\xgg", "xgg"),
  CASO("\\xg", "xg"),
  CASO("\\xz", "xz"),
  CASO("\\x4", "x4"),
  CASO("\\x", "x"),
  CASO("\\x  ", "x  "),
  CASO("\\x\t41", "x\t41"),
  CASO("\\x+1", "x+1"),
  CASO("\\x-1", "x-1"),
  CASO("\\x 1", "x 1"),
  CASO("\\x0x41", "x0x41"),

  // ---- g. Fin de string prematuro: no leer mas alla del terminador
  CASO("abc\\", "abc"),
  CASO("abc\\x", "abcx"),
  CASO("abc\\x4", "abcx4"),
  CASO("\\\\\\", "\\"),
  CASO("hola\\x4", "holax4"),

  // ---- h. El byte producido por un escape no se vuelve a interpretar
  CASO("\\x5Cn", "\\n"),
  CASO("\\x5Cx41", "\\x41"),
  CASO("\\x5Ct", "\\t"),
  CASO("\\x5C", "\\"),
  CASO("a\\x5C", "a\\"),
  CASO("\\\\x41", "\\x41"),
  CASO("\\\\n", "\\n"),
  CASO("\\\\t", "\\t"),
  CASO("\\\\\\\\", "\\\\"),
  CASO("\\x22hola\\x22", "\"hola\""),
  CASO("\\x74", "t"),
  CASO("\\x6E", "n"),

  // ---- i. Nulos embebidos: el resultado puede contener bytes nulos
  CASO("\\x00", "\x00"),
  CASO("a\\x00b", "a\x00" "b"),
  CASO("abc\\x00", "abc\x00"),
  CASO("\\x00\\x00\\x00", "\x00\x00\x00"),
  CASO("\\x00a\\x00b\\x00c", "\x00" "a\x00" "b\x00" "c"),
  CASO("hola\\x00mundohola\\x00mundohola\\x00mundohola\\x00mundohola\\x00mundo", "hola\x00mundohola\x00mundohola\x00mundohola\x00mundohola\x00mundo"),

  // ---- j. El ejemplo del enunciado
  CASO("a\\tb\\\\c\\x41\\q\\xf", "a\tb\\cAqxf"),

  // ---- k. Cada escape aislado y rodeado de vecinos (a es hexdigito, z no lo es)
  CASO("\\n", "\n"),
  CASO("a\\n", "a\n"),
  CASO("\\na", "\na"),
  CASO("a\\na", "a\na"),
  CASO("\\n\\n", "\n\n"),
  CASO("z\\nz", "z\nz"),
  CASO("\\t", "\t"),
  CASO("a\\t", "a\t"),
  CASO("\\ta", "\ta"),
  CASO("a\\ta", "a\ta"),
  CASO("\\t\\t", "\t\t"),
  CASO("z\\tz", "z\tz"),
  CASO("\\\\", "\\"),
  CASO("a\\\\", "a\\"),
  CASO("\\\\a", "\\a"),
  CASO("a\\\\a", "a\\a"),
  CASO("\\\\\\\\", "\\\\"),
  CASO("z\\\\z", "z\\z"),
  CASO("\\\"", "\""),
  CASO("a\\\"", "a\""),
  CASO("\\\"a", "\"a"),
  CASO("a\\\"a", "a\"a"),
  CASO("\\\"\\\"", "\"\""),
  CASO("z\\\"z", "z\"z"),
  CASO("\\x41", "A"),
  CASO("a\\x41", "aA"),
  CASO("\\x41a", "Aa"),
  CASO("a\\x41a", "aAa"),
  CASO("\\x41\\x41", "AA"),
  CASO("z\\x41z", "zAz"),
  CASO("\\q", "q"),
  CASO("a\\q", "aq"),
  CASO("\\qa", "qa"),
  CASO("a\\qa", "aqa"),
  CASO("\\q\\q", "qq"),
  CASO("z\\qz", "zqz"),
  CASO("\\x", "x"),
  CASO("a\\x", "ax"),
  CASO("\\xa", "xa"),
  CASO("a\\xa", "axa"),
  CASO("\\x\\x", "xx"),
  CASO("z\\xz", "zxz"),
  CASO("\\xg", "xg"),
  CASO("a\\xg", "axg"),
  CASO("\\xga", "xga"),
  CASO("a\\xga", "axga"),
  CASO("\\xg\\xg", "xgxg"),
  CASO("z\\xgz", "zxgz"),
  CASO("\\x4", "x4"),
  CASO("a\\x4", "ax4"),
  CASO("\\x4a", "J"),
  CASO("a\\x4a", "aJ"),
  CASO("\\x4\\x4", "x4x4"),
  CASO("z\\x4z", "zx4z"),
};

#define NCASOS ((int)(sizeof(casos) / sizeof(casos[0])))

// ----------------------------------------------------------------------

#define MAX_MOSTRAR 48

static void mostrar_entrada(const char *p) {
  int n = (int)strlen(p);
  int lim = n > MAX_MOSTRAR ? MAX_MOSTRAR : n;
  for (int i = 0; i < lim; i++) {
    unsigned char c = (unsigned char)p[i];
    if (c >= 0x20 && c < 0x7f) putc((int)c, stderr);
    else fprintf(stderr, "\\x%02x", c);
  }
  if (lim < n)
    fprintf(stderr, "... (%d bytes en total)", n);
}

static void mostrar_bytes(const char *p, int n) {
  int lim = n > MAX_MOSTRAR ? MAX_MOSTRAR : n;
  for (int i = 0; i < lim; i++)
    fprintf(stderr, "%02x ", (unsigned char)p[i]);
  if (lim < n)
    fputs("...", stderr);
  fputs(" |", stderr);
  for (int i = 0; i < lim; i++) {
    unsigned char c = (unsigned char)p[i];
    putc(c >= 0x20 && c < 0x7f ? (int)c : '.', stderr);
  }
  putc('|', stderr);
}

static void falla(const char *grupo, const Caso *c,
                  const char *obt, const char *detalle) {
  fflush(stdout);
  fprintf(stderr, "\n*** Error en %s: %s\n", grupo, detalle);
  fprintf(stderr, "    entrada      : ");
  mostrar_entrada(c->in);
  fprintf(stderr, "\n    su resultado : ");
  if (obt != NULL)
    mostrar_bytes(obt, c->reflen);
  else
    fprintf(stderr, "(no disponible)");
  fprintf(stderr, "\n    debio ser    : ");
  mostrar_bytes(c->ref, c->reflen);
  fprintf(stderr, "\n");
  if (obt != NULL) {
    int off = 0;
    while (off <= c->reflen && obt[off] == c->ref[off])
      off++;
    if (off > c->reflen)
      ;   // el resultado esta bien; la falla es por otro motivo
    else if (off == c->reflen)
      fprintf(stderr, "    su resultado tiene el largo equivocado\n");
    else
      fprintf(stderr, "    primera diferencia en el byte %d\n", off);
  }
  exit(1);
}

static char *clonar_string(const char *s) {
  char *p = strdup(s);
  if (p == NULL) {
    fprintf(stderr, "sin memoria\n");
    exit(1);
  }
  return p;
}

// ----------------------------------------------------------------------

static void check_desescapar(const Caso *c) {
  char *buf = clonar_string(c->in);
  desescapar(buf);
  if (memcmp(buf, c->ref, (size_t)c->reflen + 1) != 0)
    falla("desescapar", c, buf, "resultado incorrecto");
  free(buf);
}

static void check_desescapado(const Caso *c) {
  size_t nin = strlen(c->in) + 1;
  char *ent = clonar_string(c->in);
  char *r = desescapado(ent);

  if (r == NULL)
    falla("desescapado", c, NULL, "retorno NULL");

  if (memcmp(r, c->ref, (size_t)c->reflen + 1) != 0)
    falla("desescapado", c, r, "resultado incorrecto");

  // El parametro es const: no se puede modificar el string que se recibe
  if (memcmp(ent, c->in, nin) != 0)
    falla("desescapado", c, r,
          "modifico el string que recibio como parametro");

  free(r);
  free(ent);
}

static void recorrer(const char *grupo, void (*check)(const Caso *c)) {
  printf("Tests de %s: %d casos\n", grupo, NCASOS);
  for (int i = 0; i < NCASOS; i++)
    (*check)(&casos[i]);
}

// ----------------------------------------------------------------------

#ifdef OPT
#define LARGO_GRANDE (4 * 1024 * 1024)
#else
#define LARGO_GRANDE (1024 * 1024)
#endif

static void test_grande(const char *patron, char esperado) {
  int lp = (int)strlen(patron);
  int rep = LARGO_GRANDE / lp;
  char *in = malloc((size_t)rep * (size_t)lp + 1);
  char *pos = in;
  for (int i = 0; i < rep; i++) {
    memcpy(pos, patron, (size_t)lp);
    pos += lp;
  }
  *pos = '\0';

  char *r = desescapado(in);
  for (int i = 0; i < rep; i++) {
    if (r[i] != esperado) {
      fflush(stdout);
      fprintf(stderr, "\n*** Error en desescapado con un string de %d bytes: "
                      "el byte %d fue 0x%02x y debio ser 0x%02x\n",
              rep * lp, i, (unsigned char)r[i], (unsigned char)esperado);
      exit(1);
    }
  }
  if (r[rep] != '\0') {
    fflush(stdout);
    fprintf(stderr, "\n*** Error en desescapado con un string de %d bytes: "
                    "falta el caracter nulo de termino\n", rep * lp);
    exit(1);
  }
  free(r);

  desescapar(in);
  for (int i = 0; i < rep; i++) {
    if (in[i] != esperado) {
      fflush(stdout);
      fprintf(stderr, "\n*** Error en desescapar con un string de %d bytes: "
                      "el byte %d fue 0x%02x y debio ser 0x%02x\n",
              rep * lp, i, (unsigned char)in[i], (unsigned char)esperado);
      exit(1);
    }
  }
  free(in);
}

static void tests_grandes(void) {
  printf("Tests con strings de %d bytes\n", LARGO_GRANDE);
  static const char *patrones[] = { "a", "\\x41", "\\n", "\\q" };
  static const char esperados[] = { 'a', 'A', '\n', 'q' };
  for (int i = 0; i < 4; i++) {
    printf("  patron %s\n", patrones[i]);
    fflush(stdout);
    test_grande(patrones[i], esperados[i]);
  }
}

// ----------------------------------------------------------------------

// Se revisa que desescapado pida exactamente los bytes que ocupa el
// resultado, mirando el tamano del bloque que entrego malloc y no el
// maximo residente del proceso: esto ultimo depende del kernel, de la
// virtualizacion y del tamano de pagina, y no mide lo que pidio el alumno.
//
// Hay asignadores que informan exactamente los bytes pedidos (es el caso
// del sanitizador) y otros que redondean.  En vez de suponer cual es cual
// se averigua al partir: si el asignador resulta exacto se exige igualdad
// exacta, y si no se acepta el redondeo a un multiplo de ALINEACION mas
// HOLGURA.  Aun con esa tolerancia, una solucion que pide strlen(s)+1
// queda en evidencia apenas el resultado pasa de unos pocos bytes.

#define NS_MEM 4000

#define ALINEACION (2 * sizeof(size_t))
#define HOLGURA 16

#ifdef TAM_BLOQUE

// Averigua si el asignador informa el tamano pedido sin redondearlo.
static int asignador_exacto(void) {
  static const size_t pruebas[] = { 3, 17, 100, 1001, 4001 };
  for (size_t k = 0; k < sizeof pruebas / sizeof pruebas[0]; k++) {
    void *p = malloc(pruebas[k]);
    if (p == NULL) {
      fprintf(stderr, "sin memoria\n");
      exit(1);
    }
    int igual = TAM_BLOQUE(p) == pruebas[k];
    free(p);
    if (!igual) return 0;
  }
  return 1;
}

// Bloque mas grande que se acepta para una peticion de n bytes.
static size_t maximo_aceptable(size_t n, int exacto) {
  if (exacto) return n;
  return (n + ALINEACION - 1) / ALINEACION * ALINEACION + HOLGURA;
}

#endif

static void test_memoria(void) {
  printf("Test de uso de memoria de desescapado\n");
  char **res = malloc((size_t)NS_MEM * sizeof(char *));
  char *in = malloc(4 * (size_t)NS_MEM + 1);
#ifdef TAM_BLOQUE
  int exacto = asignador_exacto();
#else
  printf("  (omitido: esta plataforma no ofrece malloc_usable_size ni "
         "malloc_size)\n");
#endif

  for (int i = 1; i <= NS_MEM; i++) {
    char *pos = in;
    for (int k = 0; k < i; k++) {
      memcpy(pos, "\\x41", 4);
      pos += 4;
    }
    *pos = '\0';
    res[i - 1] = desescapado(in);
    if (res[i - 1] == NULL || strlen(res[i - 1]) != (size_t)i) {
      fflush(stdout);
      fprintf(stderr, "\n*** Error en desescapado: para %d veces \\x41 "
                      "el resultado debio tener %d bytes\n", i, i);
      exit(1);
    }

#ifdef TAM_BLOQUE
    size_t necesarios = (size_t)i + 1;
    size_t entregados = TAM_BLOQUE(res[i - 1]);
    if (entregados > maximo_aceptable(necesarios, exacto)) {
      fflush(stdout);
      fprintf(stderr, "\n*** Esta pidiendo mas memoria de la necesaria en "
                      "desescapado.\n"
                      "    Debe pedir exactamente los bytes que ocupa el "
                      "resultado, ni uno mas.\n"
                      "    Con %d veces \\x41 el resultado ocupa %zu bytes "
                      "(incluyendo el fin de string)\n"
                      "    pero su solucion pidio un bloque de %zu bytes.\n",
              i, necesarios, entregados);
      exit(1);
    }
#endif
  }

#ifdef SANITIZE
  for (int i = 1; i <= NS_MEM; i++) {
    char *r = res[i - 1];
    for (int k = 0; k <= i; k++) {
      if (*(r + k) != (k < i ? 'A' : '\0')) {
        fflush(stdout);
        fprintf(stderr, "\n*** Error en desescapado: el resultado de %d veces "
                        "\\x41 esta mal en el byte %d\n", i, k);
        exit(1);
      }
    }
  }
#endif

  for (int i = 0; i < NS_MEM; i++)
    free(res[i]);
  free(res);
  free(in);
}

// ----------------------------------------------------------------------

void test_unitario(void) {
  recorrer("desescapar", check_desescapar);
  recorrer("desescapado", check_desescapado);
  tests_grandes();
  test_memoria();
}

// ----------------------------------------------------------------------
// Benchmark

#ifdef OPT
#define LARGO_BENCH (1024 * 1024)
#define REPS_BENCH  24
#else
#define LARGO_BENCH (256 * 1024)
#define REPS_BENCH  1
#endif

static char *fuente;   // string con escapes, se desescapa una y otra vez
static char *trabajo;  // copia sobre la que trabaja desescapar

static const char *piezas[] = {
  "\\n", "\\t", "\\\\", "\\\"", "\\x41", "\\x7a", "\\xff", "\\q", "\\xg"
};

static void init_bench(void *ptr) {
  (void)ptr;
  if (fuente != NULL)
    return;
  fuente = malloc(LARGO_BENCH + 8);
  trabajo = malloc(LARGO_BENCH + 8);
  srandNum(0x5eed1234);
  char *p = fuente;
  char *lim = fuente + LARGO_BENCH;
  while (p < lim) {
    uint64_t r = randNum() >> 16;
    if (r % 10 < 3) {
      const char *pieza = piezas[(r / 10) % (sizeof(piezas) / sizeof(piezas[0]))];
      size_t n = strlen(pieza);
      memcpy(p, pieza, n);
      p += n;
    }
    else
      *p++ = (char)('a' + (r / 10) % 26);
  }
  *p = '\0';
}

static void bench_desescapar(void *ptr) {
  (void)ptr;
  size_t n = strlen(fuente) + 1;
  for (int k = 0; k < REPS_BENCH; k++) {
    memcpy(trabajo, fuente, n);
    desescapar(trabajo);
  }
}

static void bench_desescapado(void *ptr) {
  (void)ptr;
  for (int k = 0; k < REPS_BENCH; k++)
    free(desescapado(fuente));
}

void benchmark(int refs[], int nrefs) {
  init_bench(NULL);

  printf("------------------------------------------\n");
  printf("Benchmark de desescapar\n");
  run_bench(init_bench, bench_desescapar, dummyFun, NULL,
            nrefs >= 1 ? refs[0] : 0, 80, 5);

  printf("------------------------------------------\n");
  printf("Benchmark de desescapado\n");
  run_bench(init_bench, bench_desescapado, dummyFun, NULL,
            nrefs >= 2 ? refs[1] : 0, 80, 5);
}
