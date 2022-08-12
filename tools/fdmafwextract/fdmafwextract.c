#include <errno.h>
#include <fcntl.h>
#include <gelf.h>
#include <libelf.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void print_usage(void) {
    fprintf(stderr, "Usage: fdmafwextract ELF_FILENAME\n");
}

static int get_section_idx_by_name(Elf *handle, char *name) {
    /* Get total section counts. */
    size_t e_nsections;
    if (elf_getshdrnum(handle, &e_nsections) < 0) {
        int errsv = elf_errno();
        fprintf(stderr, "Failed to retrieve section counts, %s\n", elf_errmsg(errsv));

        return -1;
    }

    /* Get string table index. */
    size_t e_stridx;
    if (elf_getshdrstrndx(handle, &e_stridx) < 0) {
        int errsv = elf_errno();
        fprintf(stderr, "Failed to get string table index, %s\n", elf_errmsg(errsv));

        return -2;
    }

    /* Search sections for specific name */
    for (size_t i = 0; i < e_nsections; i++) {
        Elf_Scn *scn = elf_getscn(handle, i);
        if (scn == NULL) {
            int errsv = elf_errno();
            fprintf(stderr, "Failed to get section ID: %d, %s\n", i, elf_errmsg(errsv));

            return -3;
        }

        /* Get section header */
        GElf_Shdr  shdr_mem;
        GElf_Shdr *shdr = gelf_getshdr(scn, &shdr_mem);
        if (shdr == NULL) {
            int errsv = elf_errno();
            fprintf(stderr, "Failed to get section header ID: %d, %s\n", i, elf_errmsg(errsv));

            return -4;
        }

        /* Get section name string from table */
        const char *s_name = elf_strptr(handle, e_stridx, shdr->sh_name);
        if (s_name == NULL) {
            int errsv = elf_errno();
            fprintf(stderr, "Failed to get section name for ID: %d, %s\n", i, elf_errmsg(errsv));

            return -5;
        }

        /* Found a match */
        if (strcmp(s_name, name) == 0) {
            fprintf(stderr, "Found a match for section %s at ID: %d\n", s_name, i);
            return i;
        }
    }

    /* Loop completed with no matching sections found. */
    fprintf(stderr, "No section named %s found.\n", name);
    return -1;
}

static int dump_section_as_c_array(Elf *handle, int idx) {
    Elf_Scn *scn = elf_getscn(handle, idx);
    if (scn == NULL) {
        int es = elf_errno();
        fprintf(stderr, "Failed to get section %d: %s\n", idx, elf_errmsg(es));

        return -1;
    }

    Elf_Data *data = elf_rawdata(scn, NULL);
    if (data == NULL) {
        int es = elf_errno();
        fprintf(stderr, "Failed to get section data %d: %s\n", idx, elf_errmsg(es));

        return -2;
    }

    fprintf(stderr, "Section %d size: 0x%x, offset: 0x%x\n", idx, data->d_size, data->d_off);

    fprintf(stdout, "\n{\n");
    for(int i = 0; i < data->d_size; i++) {
        fprintf(stdout, "0x%02x, ", ((uint8_t *)data->d_buf)[i]);

        if(i % 16 == 15) {
            fprintf(stdout, "\n");
        }
    }

    fprintf(stdout, "};\n");

    return 0;
}

int main(int argc, const char *argv[]) {
    int ret = 0;

    if (argc < 2) {
        print_usage();

        ret = -1;
        return ret;
    }

    int elf_fd = open(argv[1], O_RDONLY);
    if (elf_fd < 0) {
        int es = errno;
        fprintf(stderr, "Failed to open ELF file: %s\n", strerror(es));

        ret = -2;
        return ret;
    }

    elf_version(EV_CURRENT);

    Elf *e_handle = elf_begin(elf_fd, ELF_C_READ, NULL);
    if (e_handle == NULL) {
        int es = elf_errno();
        fprintf(stderr, "ELF begin failed:  %s\n", elf_errmsg(es));

        ret = -3;
        goto err_out;
    }

    ret = get_section_idx_by_name(e_handle, ".text");
    if (ret < 0) {
        fprintf(stderr, "Failed to find text section.\n");

        ret = -4;
        goto elf_out;
    }

    ret = dump_section_as_c_array(e_handle, ret);

    ret = get_section_idx_by_name(e_handle, ".data");
    if (ret < 0) {
        fprintf(stderr, "Failed to find data section.\n");

        ret = -5;
        goto elf_out;
    }

    ret = dump_section_as_c_array(e_handle, ret);

elf_out:
    elf_end(e_handle);

err_out:
    close(elf_fd);

    return ret;
}