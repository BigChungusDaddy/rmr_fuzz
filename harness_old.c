#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "rmr.h"

/*
    SOURCE: https://github.com/o-ran-sc/ric-plt-lib-rmr/blob/master/examples/sender.c
*/
// start_example

// Sends a message and receives a message, times out after 100ms.



int main(int argc, char** argv ) {
  void*              mrc;              // msg router context
  rmr_mbuf_t*        sbuf;             // send buffer
  rmr_mbuf_t*        sbuf2 = NULL;
  rmr_mbuf_t*        rbuf;             // received buffer
  char*              listen_port = "4560";
  int                mtype       = 0;
  // char               input_payload[8192]={0};
  rmr_whid_t whid = -1;   // wormhole id for sending


  FILE *f = fopen(argv[1], "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

  char *input_payload = malloc(fsize + 1);
  fread(input_payload, fsize, 1, f);
  fclose(f);

  input_payload[fsize] = 0;

// use the string, then ...

  if( (mrc = rmr_init( listen_port, 64, RMRFL_NONE )) == NULL ) {
    fprintf( stderr, "<DEMO> unable to initialise RMR\n" );
    exit( 1 );
  }
//   strncpy(input_payload, argv[1], 1024);
//  int payload_len = sizeof(input_payload) / sizeof(char);
  sbuf = rmr_alloc_msg(mrc, 64);    // alloc 1st send buf; subsequent bufs alloc on send
  rbuf = NULL;                         // don't need to alloc receive buffer


  while( ! rmr_ready( mrc ) ) {}

  // int payload_len = read(STDIN_FILENO, input_payload, 8192);
  int payload_len = fsize;
  if (payload_len < 0) {
    perror("read");
    exit(1);
  }
  sbuf = rmr_realloc_payload(sbuf, payload_len, 0, 0);
  sbuf2 = rmr_realloc_payload(sbuf, payload_len - 1, 1, 1);
  sbuf->mtype = mtype;                      // fill in the message bits
  sbuf->len =  payload_len; // send full ascii-z string
  sbuf->state = 0;
  rmr_bytes2payload(sbuf, input_payload, payload_len);
  rmr_bytes2meid(sbuf, input_payload, payload_len);
  rmr_bytes2xact(sbuf, input_payload, payload_len);
  rmr_get_meid(sbuf, NULL);
  rmr_get_xact(sbuf, NULL);
  rmr_free_msg(sbuf2);
  sbuf = rmr_send_msg( mrc, sbuf );       // send & get next buf to fill in
  while( sbuf->state == RMR_ERR_RETRY ) { // soft failure (device busy?) retry
    sbuf = rmr_send_msg( mrc, sbuf );     // w/ simple spin that doesn't give up
  }

  rbuf = rmr_rcv_msg( mrc, rbuf); // wait for a message; timeout after 100ms
  if (rbuf->state != RMR_OK) {
    exit(1);
  }
  printf("%s", rbuf->payload);
  rmr_free_msg(sbuf);
  rmr_wh_close(mrc, whid);
  rmr_close(mrc);
  free(input_payload);

  // sbuf = rmr_send_msg( mrc, sbuf );       // send & get next buf to fill in
  // while( sbuf->state == RMR_ERR_RETRY ) { // soft failure (device busy?) retry
  //   sbuf = rmr_send_msg( mrc, sbuf );     // w/ simple spin that doesn't give up
  // }

  // rbuf = rmr_rcv_msg( mrc, rbuf); // wait for a message; timeout after 100ms
  // if (rbuf->state != RMR_OK) {
  //   exit(1);
  // }
}