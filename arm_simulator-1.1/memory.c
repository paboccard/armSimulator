/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
#include <stdlib.h>
#include "memory.h"
#include "util.h"

struct memory_data {
    int8_t *address;
    size_t size;
};

memory memory_create(size_t size) {
    memory mem;

    mem = malloc(sizeof(struct memory_data));
    if (mem) {
        mem->address = malloc(size);
        if (!mem->address) {
            free(mem);
            mem = NULL;
        }
    }
    if (mem) {
        mem->size = size;
    }
    return mem;
}

size_t memory_get_size(memory mem) {
    return mem->size;
}

void memory_destroy(memory mem) {
    free(mem->address);
    free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
    return -1;
}

int memory_read_half(memory mem, int be, uint32_t address, uint16_t *value) {
    return -1;
}

int memory_read_word(memory mem, int be, uint32_t address, uint32_t *value) {
    return -1;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
    return -1;
}

int memory_write_half(memory mem, int be, uint32_t address, uint16_t value) {
    return -1;
}

int memory_write_word(memory mem, int be, uint32_t address, uint32_t value) {
    return -1;
}
