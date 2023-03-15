#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <x86_64-linux-gnu/sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include <fcntl.h>


typedef struct {
    char debug_mode;
    char hex_mode;
    char file_name[128];
    int unit_size;
    unsigned char mem_buf[10000];
    size_t mem_count;
} state;
typedef struct {
    void *mapPtr;
    Elf32_Ehdr *header;
    int mapSize;
    int fd_map;
    Elf32_Shdr* section_header;
} Global_file;
Global_file map_file;

typedef struct {
    char *name;
    void (*fun)(state*);
}fun_desc;



void toggleDebugMode(state* s){
    if(s->debug_mode=='0'){
        printf("Debug flag now on\n");
        s->debug_mode='1';
    }
    else{
        printf("Debug flag now off\n");
        s->debug_mode='0';
    }
}
int isELFfile(Elf32_Ehdr* header){
    if(strncmp((char*)header->e_ident,(char*)ELFMAG, 4)==0){
        return 1;
    }
    return 0;
}

char* dataType(Elf32_Ehdr* header){
    switch (header->e_ident[EI_DATA]){
        case ELFDATANONE:
            return "invalid data encoding";
            break;
        case ELFDATA2LSB:
            return "2's complement, little endian";
            break;
        case ELFDATA2MSB:
            return "2's complement, big endian";
            break;
        default:
            return "NO DATA";
            break;
    }
}
void ExamineELFFile(state* s){
    printf("Enter new File Name: ");
    char newFileName[150];
    fgets(newFileName,150,stdin);
    sscanf(newFileName,"%s\n",s->file_name);
    if(s->debug_mode){
        printf("Debug: file name set to %s\n", newFileName);
    }
    if(map_file.fd_map != -1)
        close(map_file.fd_map);

    map_file.fd_map = open(s->file_name,O_RDWR);
    if(map_file.fd_map < 0){
        printf("\n\"%s \" could not open\n",
               s->file_name);
        exit(1);
    }

    struct stat sb;
    int err = fstat(map_file.fd_map, &sb);
    if(err < 0){
        printf("\n\"%s \" could not open\n",s->file_name);
        exit(1);
    }
    map_file.mapSize = sb.st_size;
    map_file.mapPtr = mmap(0,map_file.mapSize,PROT_READ | PROT_WRITE, MAP_SHARED,map_file.fd_map,0);
    map_file.header = (Elf32_Ehdr *) map_file.mapPtr;
    map_file.section_header = (Elf32_Shdr*)(map_file.mapPtr + map_file.header -> e_shoff);
    if(isELFfile(map_file.header)){
        printf("Magic: %X %X %X\n", map_file.header->e_ident[EI_MAG0], map_file.header->e_ident[EI_MAG1],
               map_file.header->e_ident[EI_MAG2]);
        printf("Data:\t %s\n", dataType(map_file.header));
        printf("Enty point address:\t\t 0x%x\n", map_file.header->e_entry);
        printf("Start of section headers:\t %d (bytes into file)\n", map_file.header->e_shoff);
        printf("Number of section headers:\t %d\n", map_file.header->e_shnum);
        printf("Size of section headers:\t %d (bytes)\n", map_file.header->e_shentsize);
        printf("Start of program headers:\t %d (bytes into file)\n", map_file.header->e_phoff);
        printf("Number of program headers:\t %d\n", map_file.header->e_phnum);
        printf("Size of program headers:\t %d (bytes)\n", map_file.header->e_phentsize);
    }
    else{
        printf("This is not ELF file\n");
        int err = munmap(map_file.mapPtr,map_file.mapSize);
        if(err != 0){
            printf("UnMapping Failed\n");
        }
        close(map_file.fd_map);
        map_file.fd_map=-1;
        map_file.header = NULL;
    }
}
void PrintSectionNames(state * s){
    // Check if the file is currently mapped
    if (map_file.fd_map == -1) {
        perror("Error: No file is currently mapped\n");
        return;
    }
    // Cast the mapped memory to a pointer to an ELF header structure
    Elf32_Shdr *sectionStringsHeader = &(map_file.section_header[map_file.header->e_shstrndx]);
    char *sectionNames = (map_file.mapPtr + sectionStringsHeader->sh_offset); // get to section names in memory

    if(s->debug_mode) {
        printf("section name offset: %u\n", sectionStringsHeader->sh_offset);
        printf("string table entry: %hu\n", map_file.header->e_shstrndx);
    }
    //print sections information:
    printf("index name           address    offset    size   type\n");
    for (int i = 0; i <map_file.header->e_shnum; i++) {
        printf("[%d]\t%-10s\t\t%x\t%x\t%x\t%x\n",i,
               (sectionNames+ map_file.section_header[i].sh_name),
               map_file.section_header[i].sh_addr,
               map_file.section_header[i].sh_offset,
               map_file.section_header[i].sh_size,
               map_file.section_header[i].sh_type);
    }
}
void PrintSymbols(state * s){
    printf("not implemented yet");
}
void RelocationTables(state * s){
    printf("not implemented yet");
}



void quit (state* s) {
    if (s->debug_mode) { printf("quitting..\n");}
    close(map_file.fd_map);
    int err = munmap(map_file.mapPtr,map_file.mapSize);
    if(err != 0){
        printf("UnMapping Failed\n");
    }
    free(s);
    exit(0);
}



void display (fun_desc menu[] , state* s){
    fprintf(stdout, "Please choose a function:\n");

    int i=0 ;
    while(menu[i].name != NULL) {
        if (s->hex_mode == 0){
            fprintf(stdout, "%d) %s\n", i, menu[i].name);
            i++;
        }
        else{
            fprintf(stdout, "%X) %s\n", i, menu[i].name);
            i++;
        }
    }
    fprintf(stdout, "Option: ");
}

int getUserInput (int bounds){
    char input[10];
    int op;
    fgets(input, 10, stdin);
    sscanf(input, "%d\n",&op);

    if (op >= 0 && op < bounds){
        fprintf(stdout, "Within bounds\n" );
        return op;
    }
    else{
        fprintf(stdout, "Not within bounds\n" );
        //exit (0);
        return -1;
    }
}

int main(int argc, char **argv){
    state* s = calloc (1 , sizeof(state));
    s->debug_mode = '0';
    s->hex_mode = '0';
    map_file.fd_map = -1;
    fun_desc menu[] = { { "Toggle Debug Mode", toggleDebugMode }, { "Examine ELF File", ExamineELFFile }, { "Print Section Names", PrintSectionNames },
                        {"Print Symbols", PrintSymbols},{"Relocation Tables", RelocationTables},{ "Quit", quit } ,{ NULL, NULL } };
    size_t bounds=0;
    while ( menu[bounds].name != NULL){
        bounds = bounds +1;
    }
    while (1) {
        display(menu , s);
        int option = getUserInput (bounds);
        if (option != -1) { menu[option].fun(s); }
        printf("\n");
    }
}
