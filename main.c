#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "out.h"

/*
    File Layout:

    +-----------------+
    | Header          | (20 bytes)
    +-----------------+
    | Section Headers | (20 * nsect bytes)
    +-----------------+
    | Emit            | (nemit bytes)
    +-----------------+
    | Reloc Table     | (8 * nrelo bytes)
    +-----------------+
    | Symbol Table    | (12 * nname bytes)
    +-----------------+
    | Strings         | (nchar bytes)
    +-----------------+
*/

static size_t g_filesz;
static char *g_filebuf;

static void dump_header(void);
static void dump_secions(void);
static void dump_reloc(void);
static void dump_names(void);
static void usage(void);
static const char *find_sec_name(int);

int main(int argc, char *argv[])
{
    int i;
    char *ifile;
    int dump_all, help;
    struct stat st;
    FILE *fp;
    size_t size;

    ifile = NULL;
    dump_all = help = 0;

    for (i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (!strcmp(arg, "-a")) {
            dump_all = 1;
            continue;
        }
        if (!strcmp(arg, "-h")) {
            help = 1;
            continue;
        }
        if (arg[0] == '-') {
            continue;
        }
        ifile = arg;
        break;
    }

    if (help) {
        usage();
        return EXIT_SUCCESS;
    }

    if (ifile == NULL) {
        fprintf(stderr, "missing input file\n");
        return EXIT_FAILURE;
    }

    memset(&st, 0, sizeof(st));
    if (stat(ifile, &st) != 0) {
        perror("stat failed");
        return EXIT_FAILURE;
    }

    g_filesz = st.st_size;
    g_filebuf = malloc(g_filesz);
    if (g_filebuf == NULL) {
        perror("malloc failed");
        return EXIT_FAILURE;
    }

    fp = fopen(ifile, "rb");
    if (fp == NULL) {
        perror("can't open file");
        return EXIT_FAILURE;
    }

    size = fread(g_filebuf, 1, g_filesz, fp);
    if (size != g_filesz) {
        fprintf(stderr, "read header fail\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    dump_header();
    if (dump_all) {
        dump_secions();
        dump_reloc();
        dump_names();
    }

    return EXIT_SUCCESS;
}

static void usage(void)
{
    puts("minidump [options] input-file");
    puts("  -a\tDump all headers");
    puts("  -h\tShow this message");
}

static void dump_header(void)
{
    struct outhead *hdr = (struct outhead *)g_filebuf;

    if (BADMAGIC(*hdr)) {
        fprintf(stderr, "bad magic\n");
        exit(EXIT_FAILURE);
    }

    printf("Header:\n");
    printf("Magic: 0x%04x\n", hdr->oh_magic);
    printf("Stamp: 0x%02x\n", hdr->oh_stamp);
    printf("Flags: 0x%02x\n", hdr->oh_flags);
    printf("# Sections: %d\n", hdr->oh_nsect);
    printf("# Relocations: %d\n", hdr->oh_nrelo);
    printf("# Names: %d\n", hdr->oh_nname);
    printf("# Emits: %d\n", hdr->oh_nemit);
    printf("# Chars: %d\n", hdr->oh_nchar);
}

static void dump_secions(void)
{
    struct outhead *hdr;
    int i;
    struct outsect *p;

    printf("\nSection:\n");

    hdr = (struct outhead *)g_filebuf;
    if (hdr->oh_nsect <= 0) {
        printf("No section found\n");
        return;
    }

    p = (struct outsect *) (g_filebuf + OFF_SECT(*hdr));
    printf("\t  Name\t    Base\t    Size\tFile Off\tFile Len\tAlign\n");
    for (i = 0; i < hdr->oh_nsect; i++, p++) {
        printf("[%4d]\t%6s\t%#8x\t%#8x\t%#8x\t%#8x\t%d\n",
                i, find_sec_name(i), p->os_base, p->os_size, p->os_foff, p->os_flen, p->os_lign);
    }
}

static const char *relo_type_misc_str(int type)
{
    if (type & RELPC) {
        return "PC";
    }
    if (type & RELBR) {
        return "BR";
    }
    if (type & RELWR) {
        return "WR";
    }
    return "  ";
}

static void dump_reloc(void)
{
    struct outhead *hdr;
    int i;
    struct outrelo *p;

    hdr = (struct outhead *)g_filebuf;

    printf("\nRelocation:\n");

    if (hdr->oh_nrelo <= 0) {
        printf("No reloc found\n");
        return;
    }

    p = (struct outrelo *) (g_filebuf + OFF_RELO(*hdr));
    printf("\tRef Type\tRel Len\tRel Etc\tRef Sect\tSym Index\tSym Addr\n");
    for (i = 0; i < hdr->oh_nrelo; i++, p++) {
        printf("[%4d]\t%#8x\t%7d\t%7s\t%8d\t%9d\t%#8x\n",
                i, p->or_type, (p->or_type & RELSZ), relo_type_misc_str(p->or_type),
                p->or_sect, p->or_nami, p->or_addr);
    }
}

static char *mkstr(const char *str)
{
    char *p;
    size_t len;
    
    len = strlen(str);
    p = malloc(len + 1);
    memcpy(p, str, len);
    return p;
}

static const char *sym_etc_str(int type)
{
    int b;

    b = type & S_ETC;
    if (b & S_SCT) {
        return "SCT";
    }
    if (b & S_LIN) {
        return "LIN";
    }
    if (b & S_FIL) {
        return "FIL";
    }
    if (b & S_MOD) {
        return "MOD";
    }
    if (b & S_COM) {
        return "COM";
    }
    if (b & S_STB) {
        return "STB";
    }
    return "   ";
}

static const char *sym_type_str(int type)
{
    int b;
    char buf[32];
    
    b = type & S_TYP;
    if (b == S_UND) {
        return "UND";
    }
    if (b == S_ABS) {
        return "ABS";
    }
    if (b == S_CRS) {
        return "CRS";
    }
    snprintf(buf, sizeof(buf), "USR%d", b - S_MIN);
    return mkstr(buf);
}

static void dump_names(void)
{
    struct outhead *hdr;
    int i;
    struct outname *p;

    hdr = (struct outhead *)g_filebuf;

    printf("\nSymbol:\n");

    if (hdr->oh_nname <= 0) {
        printf("No symbol found\n");
        return;
    }

    p = (struct outname *) (g_filebuf + OFF_NAME(*hdr));
    printf("\tFile Off\t  Type\t Etc\tExt\tDesc\tValue\tName\n");
    for (i = 0; i < hdr->oh_nname; i++, p++) {
        char *str = (char *)g_filebuf + p->on_foff;
        printf("[%4d]\t%#8x\t%6s\t%4s\t%3d\t%#4x\t%#5x\t%s\n",
                i, p->on_foff,
                sym_type_str(p->on_type), sym_etc_str(p->on_type), 
                (p->on_type & S_EXT) ? 1 : 0,
                p->on_desc, p->on_valu, 
                str);
    }
}

static const char *find_sec_name(int index)
{
    struct outhead *hdr;
    int i;
    struct outname *p;

    hdr = (struct outhead *)g_filebuf;
    if (hdr->oh_nname <= 0) {
        return "";
    }

    p = (struct outname *) (g_filebuf + OFF_NAME(*hdr));
    for (i = 0; i < hdr->oh_nname; i++, p++) {
        int t = p->on_type & S_TYP;
        if ((p->on_type & S_ETC) & S_SCT) {
            if (t >= S_MIN && t <= S_MAX && index == (t - S_MIN)) {
                return (char *)g_filebuf + p->on_foff;
            }
        }
    }

    return "";
}