#include "stdafx.h"
#include <iostream>
#include <string.h>

#include "sha.h"
#include "secblock.h"
#include "modes.h"
#include "hex.h"

#pragma comment( lib, "cryptlib.lib")

using namespace std;
using namespace CryptoPP;



void sha_test()
{
	byte message[] = "HelloWorld!";
	byte mres[256/8];

	SHA256 sha256;
	sha256.Update(message, strlen((char *)message));
	sha256.Final(mres);

	for (int i = 0; i < 16; i++)
		printf("%02X", mres[i]);

	printf("\n");


}

//
void sha_test1()
{
	CryptoPP::SHA256 hash;
	byte digest[CryptoPP::SHA256::DIGESTSIZE];
	std::string message = "abcdefghijklmnopqrstuvwxyz";

	hash.CalculateDigest(digest, (byte*)message.c_str(), message.length());

	CryptoPP::HexEncoder encoder;
	std::string output;
	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	std::cout << output << std::endl; //���sha256 ��hashֵ


	//��֤hash�Ƿ���ԭ�ַ���ƥ��
	if (hash.VerifyDigest(digest, (byte*)(message.c_str()), message.length()))
		std::cout << "��֤�ɹ�" << std::endl;
	else
		std::cout << "��֤ʧ��" << std::endl;


	digest[0] += 1; //�����޸�hash
	if (hash.VerifyDigest(digest, (byte*)(message.c_str()), message.length()))
		std::cout << "��֤�ɹ�" << std::endl;
	else
		std::cout << "��֤ʧ��" << std::endl;

}
