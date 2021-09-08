#include <glib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <malloc.h>

/* copy from qemu code */
#define PRIME32_1   2654435761U
#define PRIME32_2   2246822519U
#define PRIME32_3   3266489917U
#define PRIME32_4    668265263U
#define PRIME32_5    374761393U
#define QEMU_XXHASH_SEED 1

static inline uint32_t rol32(uint32_t word, unsigned int shift)
{
    return (word << shift) | (word >> ((32 - shift) & 31));
}

static inline uint32_t
qemu_xxhash7(uint64_t ab, uint64_t cd, uint32_t e, uint32_t f, uint32_t g)
{
    uint32_t v1 = QEMU_XXHASH_SEED + PRIME32_1 + PRIME32_2;
    uint32_t v2 = QEMU_XXHASH_SEED + PRIME32_2;
    uint32_t v3 = QEMU_XXHASH_SEED + 0;
    uint32_t v4 = QEMU_XXHASH_SEED - PRIME32_1;
    uint32_t a = ab;
    uint32_t b = ab >> 32;
    uint32_t c = cd;
    uint32_t d = cd >> 32;
    uint32_t h32;

    v1 += a * PRIME32_2;
    v1 = rol32(v1, 13);
    v1 *= PRIME32_1;

    v2 += b * PRIME32_2;
    v2 = rol32(v2, 13);
    v2 *= PRIME32_1;

    v3 += c * PRIME32_2;
    v3 = rol32(v3, 13);
    v3 *= PRIME32_1;

    v4 += d * PRIME32_2;
    v4 = rol32(v4, 13);
    v4 *= PRIME32_1;

    h32 = rol32(v1, 1) + rol32(v2, 7) + rol32(v3, 12) + rol32(v4, 18);
    h32 += 28;

    h32 += e * PRIME32_3;
    h32  = rol32(h32, 17) * PRIME32_4;

    h32 += f * PRIME32_3;
    h32  = rol32(h32, 17) * PRIME32_4;

    h32 += g * PRIME32_3;
    h32  = rol32(h32, 17) * PRIME32_4;

    h32 ^= h32 >> 15;
    h32 *= PRIME32_2;
    h32 ^= h32 >> 13;
    h32 *= PRIME32_3;
    h32 ^= h32 >> 16;

    return h32;
}

typedef struct key {
    int bus;
    int devfn;
} key;

typedef struct value {
    int base;
    int bar;
} value;

static guint key_hash(gconstpointer v)
{
    key *k = (key *)v;

    return qemu_xxhash7((uint64_t)k->bus, k->devfn, 0, 0, 0);
}

static gboolean key_equal(gconstpointer v1, gconstpointer v2)
{
    key *k1 = (key *)v1;
    key *k2 = (key *)v2;

    return (k1->bus == k2->bus) && (k1->devfn == k2->devfn);
}

int main()
{
    	GHashTable *hash_table;
    	GHashTable *int_table;
	value v, *p_v, *p;
	key k, *p_k;
	int int_k = 2;

    	hash_table = g_hash_table_new_full(key_hash, key_equal, g_free, g_free);

	p_v = malloc(sizeof(v)); p_v->base = 1; p_v->bar = 3;
	p_k = malloc(sizeof(k)); p_k->bus = 0x10; p_k->devfn = 0x75;

	g_hash_table_insert(hash_table, p_k, p_v);

	k.bus = 0x10; k.devfn = 0x75;
	p_v = g_hash_table_lookup(hash_table, &k);
	if (p_v)
		printf("found!\n");
	else
		printf("not found!\n");

	k.bus = 0x20; k.devfn = 0x75;
	p_v = g_hash_table_lookup(hash_table, &k);
	if (p_v)
		printf("found!\n");
	else
		printf("not found!\n");

	g_hash_table_remove_all(hash_table);

	/* start to test g_int_hash */
    	int_table = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, g_free);
	p_v = malloc(sizeof(v)); p_v->base = 1; p_v->bar = 3;
	g_hash_table_insert(int_table, &int_k, p_v);
	p = g_hash_table_lookup(int_table, &int_k);
	if (p)
		printf("found!\n");
	else
		printf("not found!\n");
	g_hash_table_remove_all(int_table);

	return 0;
}

// sudo apt-get install libglib2.0-dev
// gcc test.c `pkg-config --cflags --libs glib-2.0`
