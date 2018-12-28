#include "stdafx.h"
#include "randpool.h"
#include "rsa.h"
#include "osrng.h"
#include "hex.h"
#include "files.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
using namespace CryptoPP;

#pragma comment(lib, "cryptlib.lib")

//------------------------
// ��������
//------------------------
void GenerateRSAKey(unsigned int keyLength, const char *privFilename, const char *pubFilename, const char *seed);
string RSAEncryptString(const char *pubFilename, const char *seed, const char *message);
string RSADecryptString(const char *privFilename, const char *ciphertext);


//------------------------
// ������
//------------------------
void rsa_test()
{
	char priKey[128] = { 0 };
	char pubKey[128] = { 0 };
	char seed[1024] = { 0 };

	// ���� RSA ��Կ��
	strcpy(priKey, "pri");  // ���ɵ�˽Կ�ļ�, ��ͬ��Ŀ¼��
	strcpy(pubKey, "pub");  // ���ɵĹ�Կ�ļ�
	strcpy(seed, "seed");
	GenerateRSAKey(1024, priKey, pubKey, seed);

	// RSA �ӽ���
	char message[1024] = { 0 };
	cout << "Origin Text:\t" << "Hello World!" << endl << endl;
	strcpy(message, "Hello World!");
	string encryptedText = RSAEncryptString(pubKey, seed, message);  // RSA ����
	cout << "Encrypted Text:\t" << encryptedText << endl << endl;
	string decryptedText = RSADecryptString(priKey, encryptedText.c_str());  // RSA ����
	cout << "Decrypted Text:\t" << decryptedText << endl << endl;
}



//------------------------
// ����RSA��Կ��
//------------------------
void GenerateRSAKey(unsigned int keyLength, const char *privFilename, const char *pubFilename, const char *seed)
{
	//RandomPool randPool;
	AutoSeededRandomPool randPool;
	//randPool.Put((byte *)seed, strlen(seed));
	//randPool.

	RSAES_OAEP_SHA_Decryptor priv(randPool, keyLength);
	HexEncoder privFile(new FileSink(privFilename));
	priv.DEREncode(privFile);
	privFile.MessageEnd();

	RSAES_OAEP_SHA_Encryptor pub(priv);
	HexEncoder pubFile(new FileSink(pubFilename));

	pub.DEREncode(pubFile);

	pubFile.MessageEnd();
}



//------------------------
// RSA����
//------------------------
string RSAEncryptString(const char *pubFilename, const char *seed, const char *message)
{
	FileSource pubFile(pubFilename, true, new HexDecoder);

	RSAES_OAEP_SHA_Encryptor pub(pubFile);
	AutoSeededRandomPool randPool;
	/*RandomPool randPool;*/
	//randPool.Put((byte *)seed, strlen(seed));

	string result;
	StringSource(message, true, new PK_EncryptorFilter(randPool, pub, new HexEncoder(new StringSink(result))));
	return result;
}



//------------------------
// RSA����
//------------------------
string RSADecryptString(const char *privFilename, const char *ciphertext)
{
	FileSource privFile(privFilename, true, new HexDecoder);
	RSAES_OAEP_SHA_Decryptor priv(privFile);
	string result;

	AutoSeededRandomPool  randPool;
	StringSource(ciphertext, true, new HexDecoder(new PK_DecryptorFilter(randPool, priv, new StringSink(result))));
	return result;

}
