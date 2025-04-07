#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <time.h>

#include <rmr/rmr.h>

/*
    SOURCE: https://github.com/o-ran-sc/ric-plt-lib-rmr/blob/master/examples/sender.c
*/
// start_example

// Sends a message and receives a message, times out after 100ms.

int main( int argc, char** argv ) {
  void*              mrc;              // msg router context
  rmr_mbuf_t*        sbuf;             // send buffer
  rmr_mbuf_t*        rbuf;             // received buffer
  char*              listen_port = "4560";
  int                mtype       = 0;
  char               input_payload[1024]={0};
  
//   if (argc != 2) {
//     fprintf(stderr, "Wrong number of arguments\n");
//     exit(1);
//   }

  if( (mrc = rmr_init( listen_port, 1024, RMRFL_NOTHREAD )) == NULL ) {
    fprintf( stderr, "<DEMO> unable to initialise RMR\n" );
    exit( 1 );
  }
  int payload_len = read(STDIN_FILENO, input_payload, 1024);
//   strncpy(input_payload, argv[1], 1024);
//  int payload_len = sizeof(input_payload) / sizeof(char);
  sbuf = rmr_alloc_msg(mrc, 1024);    // alloc 1st send buf; subsequent bufs alloc on send
  rbuf = NULL;                         // don't need to alloc receive buffer

  while( ! rmr_ready( mrc ) ) {        // must have route table
    sleep( 1 );                        // wait til we get one
  }
  sbuf->mtype = mtype;                      // fill in the message bits
  sbuf->len =  payload_len + 1; // send full ascii-z string
  sbuf->state = 0;
  memcpy(sbuf->payload, input_payload, sbuf->len);
  sbuf = rmr_send_msg( mrc, sbuf );       // send & get next buf to fill in
  while( sbuf->state == RMR_ERR_RETRY ) { // soft failure (device busy?) retry
    sbuf = rmr_send_msg( mrc, sbuf );     // w/ simple spin that doesn't give up
  }

  rbuf = rmr_rcv_msg( mrc, rbuf); // wait for a message; timeout after 100ms
  if (rbuf->state != RMR_OK) {
    exit(1);
  }
}
