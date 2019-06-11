#include "encrypt.h"
#include "checkentropy.h"

int start_encrypt(std::string in, char *out)
{
    check_entropy();

    if (sodium_init() == -1) 
    {   
        return -1;
    }

    char hexbuf[1024];

	int rc;

	Machine *backdoor = (Machine*) malloc(sizeof(Machine));
    backdoor->name = "backdoor";
    sodium_hex2bin(backdoor->public_key, crypto_box_PUBLICKEYBYTES,
                   PKEY, 2*crypto_box_PUBLICKEYBYTES+1, NULL, NULL, NULL);
    
    sodium_hex2bin(	backdoor->secret_key, crypto_box_PUBLICKEYBYTES,
                   SKEY, 2*crypto_box_PUBLICKEYBYTES+1, NULL, NULL, NULL);

	Machine *controller = new_machine("controller");
	uint8_t *msg = new uint8_t[in.length() + 1];
    strcpy((char*)msg, in.c_str());

	uint8_t nonce[crypto_box_NONCEBYTES];
	randombytes(nonce, crypto_box_NONCEBYTES);

	print_machine(backdoor);
	print_machine(controller);

	printf("message: %s\n", msg);

	uint8_t encrypted[1000];
	rc = encrypt(encrypted, controller->public_key, backdoor->secret_key, nonce, msg, in.length()+1);
	if( rc < 0 ) 
    {
		return 1;
	}
	//printf("encrypted: %s\n", to_hex(hexbuf, encrypted, rc ));
    printf("encrypted: %s\n", sodium_bin2hex(hexbuf, 2*rc+1, encrypted, rc));
	out = (char*) malloc(rc);
	memcpy(out, hexbuf, rc);
	return rc;

	uint8_t decrypted[1000];
	decrypt(decrypted, backdoor->public_key, controller->secret_key, nonce, encrypted, rc);
	if( rc < 0 ) 
    {
		return 1;
	}

	decrypted[rc] = '\0';
    printf("decrypted: %s\n", decrypted);

    return rc;
}

int start_decrypt(char* data, int len)
{
// 	check_entropy();

//     if (sodium_init() == -1) 
//     {   
//         return -1;
//     }

//     char hexbuf[1024];

// 	int rc;

// 	Machine *backdoor = (Machine*) malloc(sizeof(Machine));
//     backdoor->name = "backdoor";
//     sodium_hex2bin(backdoor->public_key, crypto_box_PUBLICKEYBYTES,
//                    PKEY, 2*crypto_box_PUBLICKEYBYTES+1, NULL, NULL, NULL);
    
//     sodium_hex2bin(backdoor->secret_key, crypto_box_PUBLICKEYBYTES,
//                    SKEY, 2*crypto_box_PUBLICKEYBYTES+1, NULL, NULL, NULL);

// 	Machine *controller = new_machine("controller");

// 	uint8_t nonce[crypto_box_NONCEBYTES];
// 	randombytes(nonce, crypto_box_NONCEBYTES);

// 	uint8_t encrypted[1000];
// 	uint8_t decrypted[1000];
// 	rc = decrypt(decrypted, backdoor->public_key, controller->secret_key, nonce, encrypted, rc);
// 	if( rc < 0 ) 
//     {
// 		return 1;
// 	}

// 	decrypted[rc] = '\0';
//     printf("decrypted: %s\n", decrypted);


}

void randombytes(uint8_t buffer[], unsigned long long size)
{
	int fd;

	fd = open( "/dev/urandom", O_RDONLY );
	if( fd < 0 ) {
		fprintf( stderr, "Failed to open /dev/urandom\n" );
		exit(1);
	}

	int rc;
	if( (rc = read( fd, buffer, size )) >= 0 ) {
		close( fd );
	}
}

char* to_hex( char hex[], const uint8_t bin[], size_t length )
{
	int i;
	uint8_t *p0 = (uint8_t *)bin;
	char *p1 = hex;

	for( i = 0; i < length; i++ ) {
		snprintf( p1, 3, "%02x", *p0 );
		p0 += 1;
		p1 += 2;
	}

	return hex;
}


int is_zero( const uint8_t *data, int len )
{
	int i;
	int rc;

	rc = 0;
	for(i = 0; i < len; ++i) {
		rc |= data[i];
	}

	return rc;
}

int encrypt(uint8_t encrypted[], const uint8_t pk[], const uint8_t sk[], const uint8_t nonce[], const uint8_t plain[], int length)
{
	uint8_t temp_plain[MAX_MSG_SIZE];
	uint8_t temp_encrypted[MAX_MSG_SIZE];
	int rc;

	printf("encrypt\n", length);

	if(length+crypto_box_ZEROBYTES >= MAX_MSG_SIZE) {
		return -2;
	}

	memset(temp_plain, '\0', crypto_box_ZEROBYTES);
	memcpy(temp_plain + crypto_box_ZEROBYTES, plain, length);

	rc = crypto_box(temp_encrypted, temp_plain, crypto_box_ZEROBYTES + length, nonce, pk, sk);

	if( rc != 0 ) {
		return -1;
	}

	if( is_zero(temp_plain, crypto_box_BOXZEROBYTES) != 0 ) {
		return -3;
	}

	memcpy(encrypted, temp_encrypted + crypto_box_BOXZEROBYTES, crypto_box_ZEROBYTES + length);

	return crypto_box_ZEROBYTES + length - crypto_box_BOXZEROBYTES;
}

int decrypt(uint8_t plain[], const uint8_t pk[], const uint8_t sk[], const uint8_t nonce[], const uint8_t encrypted[], int length)
{
	uint8_t temp_encrypted[MAX_MSG_SIZE];
	uint8_t temp_plain[MAX_MSG_SIZE];
	int rc;

	printf("decrypt\n");

	if(length+crypto_box_BOXZEROBYTES >= MAX_MSG_SIZE) {
		return -2;
	}

	memset(temp_encrypted, '\0', crypto_box_BOXZEROBYTES);
	memcpy(temp_encrypted + crypto_box_BOXZEROBYTES, encrypted, length);

	rc = crypto_box_open(temp_plain, temp_encrypted, crypto_box_BOXZEROBYTES + length, nonce, pk, sk);

	if( rc != 0 ) {
		return -1;
	}

	if( is_zero(temp_plain, crypto_box_ZEROBYTES) != 0 ) {
		return -3;
	}

	memcpy(plain, temp_plain + crypto_box_ZEROBYTES, crypto_box_BOXZEROBYTES + length);

	return crypto_box_BOXZEROBYTES + length - crypto_box_ZEROBYTES;
}

Machine *new_machine(char* name)
{
	Machine* machine;

	machine = (Machine*) malloc(sizeof(Machine));
	machine->name = name;

	crypto_box_keypair(machine->public_key, machine->secret_key);

	return machine;
}

void print_machine(Machine *machine)
{
	char phexbuf[2*crypto_box_PUBLICKEYBYTES+1];
	char shexbuf[2*crypto_box_SECRETKEYBYTES+1];

	printf("username: %s\n", machine->name);
	//printf("public key: %s\n", to_hex(phexbuf, machine->public_key, crypto_box_PUBLICKEYBYTES ));
    printf("public key  %s\n", sodium_bin2hex(phexbuf, 2*crypto_box_PUBLICKEYBYTES+1, machine->public_key, crypto_box_PUBLICKEYBYTES));
	//printf("secret key: %s\n\n", to_hex(shexbuf, machine->secret_key, crypto_box_SECRETKEYBYTES ));
    printf("secret key: %s\n\n", sodium_bin2hex(shexbuf, 2*crypto_box_SECRETKEYBYTES*2+1, machine->secret_key, crypto_box_SECRETKEYBYTES ));
}