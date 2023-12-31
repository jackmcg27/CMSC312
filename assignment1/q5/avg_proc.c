#include <rpc/rpc.h>
#include "avg.h"
#include <stdio.h>
#include <math.h>

/* run locally on 'server' called by a remote client. */
static double sum_avg;

/* 
 * routine notice the _1 the version number 
 * notice the client handle, not sued here but needs to be 
 * a parameter 
 */
input_data * average_1(input_data *input, CLIENT *client) 
  {

  /* input is paramters were marshalled by genrated routine */
  /*  a pointer to a double, is set to begining of data array  */
  double *dp = input->input_data.input_data_val;

  int hold;

  for (int i = 0; i < input->input_data.input_data_len; i++) {
      for (int j = i + 1; j < input->input_data.input_data_len; j++) {
        if(*(dp+i) > *(dp+j)) {
          hold = *(dp+i);
          *(dp+i) = *(dp+j);
          *(dp+j) = hold;
        }
      }
  }

  return( input );
}

/* 
 * server stub 'average_1_svc function handle called in avg_svc that was
 * generated by rpcgen 
 * FYI:
 *   result = (*local)((char *)&argument, rqstp);
 *   where local is (char *(*)(char *, struct svc_req *)) average_1_svc;
 */
 
input_data * average_1_svc(input_data *input, struct svc_req *svc) 
  {
  CLIENT *client;
  return( average_1( input, client) );
  }
