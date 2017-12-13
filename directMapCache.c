#include <iostream>
#include <stdlib.h>

/*Project 8
 * Ryan Oglesby (951181731)
 *
 * DIRECT MAPPED CACHE IMPLEMENTATION
 *
 * */

/* Define our cache block struct -Implemented from Zach's Lab.  */
typedef struct{
    unsigned char valid;
    unsigned int tag;
    unsigned char value[4];} _cacheBlock;


/*My function that i used from assignment one, altered to suit our needs.
  It will just print one byte at a time as opposed to 2.
 * */
typedef unsigned char *byte_pointer;


void show_bytes(byte_pointer start){
    int i;
    for (i=0; i<1; i++)
        printf(" %.2x ", start[i]);

}

void show_short(short int x){
    show_bytes((byte_pointer) &x);
}

/*
   writeCache function will take a address and value, then get the set bits of
   the address using shifts to delete the tag and offsetbits.
   It will then check if the block is valid, if it is that means a block is already
   there and we need to evict it.
   If not then we just continue to write onto the block and announce it
   also set the block as valid to let us know we stored a block there.
 * */
void writeCache(_cacheBlock* array,unsigned address,unsigned val){
    unsigned setBits;
    unsigned int a,b,c,d;


    setBits = address << 26; //Get rid of tag bits
    setBits = setBits >> 28; //Get rid of the offset bits, only left with SET BITS



    //printf("SET BITS: %u \n",setBits);



            if (array[setBits].valid == 1){  //Already a block in here?
                printf("evicting block ");
                printf("- set: %d ",setBits);
                printf("- tag: %d ",array[setBits].tag);
                printf("- valid: %d ", array[setBits].valid);
                printf("- value: ");

                for (int z = 0; z<4; z++){
                    show_short(array[setBits].value[z]);
                }
                printf("\n");
                /* Grab the initial byte values to store into the char[4]
                 */
                a = (val & 0xFF); //First byte
                b = ((val>>8)&0xFF); // Second byte
                c = ((val>>16)&0xFF); // Third
                d = ((val>>24)&0xFF); // Fourth
                array[setBits].valid = 1;
                // Set them to our values
                array[setBits].value[0] = a;
                array[setBits].value[1] = b;
                array[setBits].value[2] = c;
                array[setBits].value[3] = d;

                printf("wrote set: %d ",setBits);


                unsigned tagBits = address >> 6; // Get tag value


                array[setBits].tag = tagBits;   // Set tag value

                printf("- tag: %d ",array[setBits].tag);
                printf(" - valid: %d",array[setBits].valid);
                printf(" - value: ");
                for (int z = 0; z<4; z++){
                    show_short(array[setBits].value[z]);
                }
                printf("\n");
            }
            // Write a block if Nothing here.
            else{

                a = (val & 0xFF); //First byte
                b = ((val>>8)&0xFF); // Second byte
                c = ((val>>16)&0xFF); // Third
                d = ((val>>24)&0xFF); // Fourth

                array[setBits].valid = 1;

                array[setBits].value[0] = a;
                array[setBits].value[1] = b;
                array[setBits].value[2] = c;
                array[setBits].value[3] = d;

                printf("Wrote set : %u ",setBits);

                unsigned tagBits = address >> 6; //Get tag value


                array[setBits].tag = tagBits;

                printf("- tag: %u ",array[setBits].tag);
                printf(" - valid: %d",array[setBits].valid);
                printf(" - value: ");
                for (int z = 0; z<4; z++){
                    show_short(array[setBits].value[z]);
                }
                printf("\n");
            }

        }




/* readCache will take a address and get the setBits, offsetBits and tagBits using
 * arithmetic binary shifts.
 * Once it has the setBits it will search the block with that set, if it finds
 * a valid block it will show the byte found in that offsetBit Cache location
 * If we the tagBits don't match though, its not a hit , its a miss.
 * */

void readCache(_cacheBlock* array,unsigned address){

    unsigned setBits = address << 26; //Get rid of tag bits
    setBits = setBits >> 28; //Get rid of the offset bits, only left with SET BITS

    unsigned tagBits = address >> 6;

    unsigned offsetBits = address << 30;
    offsetBits = offsetBits >> 30;


    printf("Looking for set: %d",setBits);
    printf(" - tag: %d \n",tagBits);

      // Found the set we are looking for
            if(array[setBits].valid == 1){   // Make sure its valid
                printf("Found set: ");
                printf("%d ",setBits);
                printf(" - tag: %d",array[setBits].tag);
                printf(" - offset: %d",offsetBits);
                printf(" - valid: %d", array[setBits].valid);

                printf(" - value: ");

                show_short(array[setBits].value[offsetBits]);

                printf("\n");

                if(array[setBits].tag != tagBits){
                    printf("tags don't match - miss! \n");
                }else{
                    printf("Hit!\n");
                }

            }

            else{                      // Not valid, MISS
                printf("BNF: BLOCK NOT FOUND! - miss!\n");
            }
        }



/*printCache simply loops through our array and whenever we encounter a block with a validated bit
 * we will print the values.. easy!
 *
 * */

void printCache(_cacheBlock* array,int size){
    for (int i = 0; i<size;i++){

        if (array[i].valid == 1){

            printf("Set: ");
            printf("%d ",i);
            printf(" - tag: %d",array[i].tag);
            printf(" - valid: %d", array[i].valid);
            printf(" - value: ");
            for (int z = 0; z<4; z++){
                show_short(array[i].value[z]);
            }
            printf("\n");


        }
    }

}



int main() {

    /* Initialize our array pointer to our 16 blocks */
    _cacheBlock *array;
    array = (_cacheBlock *)malloc(sizeof(_cacheBlock)*16);

    /*Clear our cache block , and set the validation tags to false.(0)*/

    for (int i = 0; i<16;i++){
        //unsigned int x = array[i].tag;
        array[i].tag = 0;
        array[i].valid = 0;
        for (int z = 0; z<4; z++){
            array[i].value[z] = 0;
        }
    }

    char input;
    unsigned address;
    unsigned value;
    // Implemented From lab
    do {


        printf("VALID COMMANDS: 'r' -> Read an Hex address | 'w' -> Write to an Hex address with a hex value ");
        printf("'p' -> Print the cache contents | 'q' -> QUIT");
        scanf(" %c", &input);
        printf("\n");

        if (input == 'r'){
            printf("Enter 32 bit unsigned hex address");
            scanf(" %x",&address);
            printf("\n");

            readCache(array, address);
        }else if (input == 'w'){
            printf("Enter 32 bit unsigned hex address");
            scanf(" %x",&address);
            printf("\n");


            printf("Enter 32 bit unsigned hex value to store");
            scanf(" %x",&value);
            printf("\n");

            writeCache(array,address,value);
        }else if (input == 'p'){
            int _size = 16;  // We know how many blocks we have.
            printCache(array,_size);
        }else if (input != 'q'){
            printf("Please enter a valid character..\n");

        }






    }while(input != 'q');


    printf("\n GOODBYE!..");


    return 0;
}
