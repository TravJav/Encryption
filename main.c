//
//  main.c
//  seguro
//
//  Created by Travis Haycock on 2017-01-07.
//  Copyright © 2017 Travis Haycock. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <string.h>
#include <openssl/evp.h>
#include <sys/stat.h>
#include <errno.h>
#include <openssl/conf.h>

#ifdef _UNIX_
#include <unistd.h>
#endif

/*
 Copyright (C) <2017> <TRAVIS HAYCOCK>
 All rights reserved.
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation  and/or other materials provided with the distribution.
 3. Neither the names of the copyright holders nor the names of any
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */


unsigned char key[32]; // key which is not hard-coded for obvious reasons, key is in users head!
unsigned char vec[16] = "nombUjH*9)r*7K]{,.?";//use as initialization Vector ideally in production this should be secret.

void encrypt_process(); //define methods for calling
void decrypt_process();






struct class { //template for future implementation of advanced functions
    char input_file_path[100]; // input file to methods
    char output_file_path[100]; // input file to methods
    
};




/*=========================================================================|
  Encryption Method below, This Will Take The File And Use The Keys And IV  
  To Encrypt The Data/File. We Use Long Here Because Int Only Is Good For <2GB
  Data Size And Will Get ERRORS If File Is Larger Than >= 2GB Which Is As Common
 as gold-diggers.
 *=========================================================================*/
 void encrypt_process(){

     const char input_file_path[150];
     const char str2[150] = "Ciph.txt"; //hold the file ending name to let user know it's ciphered
     
  
     
     printf("Enter Input File Path: ");
     scanf("%s", input_file_path);

   printf("Entered File Name: %s\n", input_file_path);
     

   FILE *Input_File;
   FILE *Output_file;

    
    /* OPENSSL ERROR CHECK METHODS
     */
     
    ERR_load_crypto_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);

     
     
     Input_File =fopen(input_file_path, "rb");
    if (!(Input_File = fopen(input_file_path, "r"))){ // Check to see if File exist if not console will print error
        perror("errno");

   }else
    
    printf("Input Password Key You Will Use");
    scanf("%s", &key); // key will be used for encryption process
     
     
   
     char *result = malloc(strlen(input_file_path)+strlen(str2)+1);//check for malloc errors in production
     
     if(result == NULL){
     fprintf(stderr, "malloc() failed: insufficient memory!\n");
    
     }else
     
 
    strcpy(result, input_file_path); // put the two strings together so the ending is our desired cipher.txt
     strcat(result, str2);

    Output_file = fopen(result, "wb");//File to be written; cipher text
     
    ERR_load_crypto_strings();   //ERROR check for openssl lib
    /* Initialise the library */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
    
    fseek(Input_File, 0, SEEK_END);
    unsigned long len = ftell(Input_File);
    fseek(Input_File, 0, SEEK_SET);
    
    
    printf("length of the file is : %lu", len);
    unsigned long outLen1 = 0;
    unsigned long outLen2 = 0;
    unsigned char *indata = malloc(len);
    unsigned char *outdata = malloc(len);
    
    fread(indata,sizeof(char),len, Input_File);//Read Entire File
    
    
    
    /*-----------------------------------------------------------------*\
     <|||    Set Up Encryption As Defined in OPENSSH using their syntax etc.>>>
     *-----------------------------------------------------------------*/
     
    //initiating cipher
    EVP_CIPHER_CTX ctx;
    
    //EVP_EncryptInit(&ctx,EVP,p_KEY,p_IV);
    
    //EVP_EncryptInit(&ctx,EVP_aes_128_cbc(),key,vec);
     
     EVP_EncryptInit(&ctx,EVP_aes_256_gcm(),key,vec);
     EVP_EncryptUpdate(&ctx,outdata,&outLen1,indata,len);

    
    EVP_EncryptFinal(&ctx,outdata + outLen1,&outLen2);
    fwrite(outdata,sizeof(char),outLen1 + outLen2,Output_file);
    
    fclose(Input_File); // free all pointers and clean up
    fclose(Output_file);
     free(result);
    Input_File = NULL;

   
    printf("\n Encryption Process Complete");
     printf("\n ***** Do You Have More Requests? y/n*******");
     char choose[1];
     scanf("%c",&choose);
     if(strcmp(choose, "y")== 0){
         printf("\n***** e for encrypt or d for decrypt");
         char user_choose[1];
         scanf("%s", &choose);
         if(strcmp(user_choose, "e")== 0){
            // encrypt_process(char *file_path);
         } else  if(strcmp(user_choose, "d")== 0){
             decrypt_process();
         }
     }
 }


 



/*=========================================================================|
 Decryption Method below, This Will Take The File And Use The Keys And IV
 To Decrypt The Data/File. We Use Long Here Because Int Only Is Good For <= 2GB
 Data Size And Will Get ERRORS If File Is Larger Than 2GB Which Is As Common
 as gold-diggers
 *=========================================================================*/
void decrypt_process(){
 
    
    const char input_file_path[150];
    const char str2[150] = "-Decrypted.txt"; //hold the file ending name to let user know it's ciphered
    
    
    
    printf("Enter Input File Path Where Your Encrypted File Path Resides: ");
    scanf("%s", input_file_path);
    
    printf(" \n Entered File Name: %s", input_file_path);
    
    
    
    
    FILE *Input_File;
    FILE *Output_file;
    ERR_load_crypto_strings();   //ERROR check for openssl lib
    /* Initialise the library */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
    
   Input_File =fopen(input_file_path, "rb");
    
    
    //if (!(Input_File = fopen(input_file_path, "r"))){ // Check to see if File exist
        //perror("errno");
        
    //} else
        
     printf("\n ******Input Password Key You Used*******");
         scanf("%s", &key);
    
    
    
    
    char *result = malloc(strlen(input_file_path)+strlen(str2)+1);//check for malloc errors in production
    
    if(result == NULL){
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        
    }else
        
        
        strcpy(result, input_file_path); // put the two strings together so the ending is our desired cipher.txt
        strcat(result, str2);
    
    
    
    
  Output_file = fopen(result, "wb");
    
    ERR_load_crypto_strings();//ERROR check for openssl lib
    /* Initialise the library */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
    
    
    
    fseek(Input_File, 0, SEEK_END);
    unsigned long len = ftell(Input_File); // use long for files to be safe cause int is =<2GB
    fseek(Input_File, 0, SEEK_SET); // sent back to original so it doesnt read 0
    
    printf("\n *******length of the file is : %lu*****", len); //xcode underlines this?
    
    unsigned long outLen1 = 0;
    unsigned long outLen2 = 0;
    unsigned char *indata = malloc(len);
    unsigned char *outdata = malloc(len);
    
    fread(indata,sizeof(char),len, Input_File);//Read Entire File
    
    /*-----------------------------------------------------------------*\
     <|||    Set Up Decryption As Defined in OPENSSH using their syntax etc.>>>
     *-----------------------------------------------------------------*/
    
    //initiating decrypt
    EVP_CIPHER_CTX ctx;

  
    EVP_DecryptInit(&ctx,EVP_aes_256_gcm(),key,vec);
    EVP_DecryptUpdate(&ctx,outdata,&outLen1,indata,len);
    EVP_DecryptFinal(&ctx,outdata + outLen1,&outLen2);
    fwrite(outdata,sizeof(char),outLen1 + outLen2,Output_file);
    
    
    fclose(Input_File); // free all pointers and clean up
    fclose(Output_file);
    free(result);
    Input_File = NULL; //extra care

    // openssl error callings
    printf("\n *****Decryption Process Complete");
    printf("\n *****Do You Have More Requests? y/n");
    char choose[1];
    scanf("%c",&choose);
    if(strcmp(choose, "y")== 0){
        printf("\n***** e for encrypt or d for decrypt");
        char user_choose[1];
        scanf("%s", &choose);
        if(strcmp(user_choose, "e")== 0){
        // encrypt_process();
        } else  if(strcmp(user_choose, "d")== 0){
          decrypt_process();
        }
    }
    

}



// main entry point
int main(int argc, const char * argv[]) {
    
    char option[5]; //local buffer to get input
   printf("\n **********Welcome To Encryption And Decryption Services!*********");
    
    printf("\n *******Type e for ecryption or d for decryption (more options later)***** ");
    
    scanf("%s", option);
    
    if(strcmp(option, "e")== 0){
    printf("*********Encryption Process Initiated ");
    
     encrypt_process(); //======= encryption method
        
    }else if(strcmp(option,"d")== 0){
    printf("********Decryption Process Initiated********");
        decrypt_process(); //====== decryption method
    
    }
    
    return 0;
}

