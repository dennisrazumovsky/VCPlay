#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "vcplay_api.h"

int
main(void)
{
	vc_api_opts api_opts;
	int error;

	error = vc_api_init(/* verbose */1);
	assert(error == 0);

	memset(&api_opts, 0, sizeof(api_opts));
	api_opts.tc_device = "/dev/vn1s0";
	api_opts.tc_passphrase = "apitest2";
	api_opts.tc_keyfiles = NULL;
	api_opts.tc_keyfiles_hidden = NULL;
	api_opts.tc_size_hidden_in_bytes = 12000*512;
	api_opts.tc_passphrase_hidden = "apihidden";
	api_opts.tc_cipher = "AES-256-XTS,TWOFISH-256-XTS,SERPENT-256-XTS";
	api_opts.tc_cipher_hidden = "SERPENT-256-XTS,TWOFISH-256-XTS";
	api_opts.tc_prf_hash = "whirlpool";
	api_opts.tc_prf_hash_hidden = "RIPEMD160";

	error = vc_api_create_volume(&api_opts);
	if (error)
		printf("API ERROR: %s\n", vc_api_get_error_msg());
	assert(error == 0);

	error = vc_api_uninit();
	assert(error == 0);

	error = vc_api_init(/*verbose */ 1);
	assert(error == 0);

	api_opts.tc_passphrase = NULL;
	api_opts.tc_map_name = "dragonfly-test";
	api_opts.tc_password_retries = 5;
	api_opts.tc_interactive_prompt = 1;
	api_opts.tc_prompt_timeout = 5;
	error = vc_api_map_volume(&api_opts);
	if (error)
		printf("API MAP ERROR: %s\n", vc_api_get_error_msg());
	assert(error == 0);

	system("dmsetup ls");
	vc_api_unmap_volume(&api_opts);

	error = vc_api_uninit();
	assert(error == 0);

	return 0;
}
