/**
 * @author INRIA Lorraine - 1999
 * @author Sylvestre LEDRU <sylvestre.ledru@inria.fr> INRIA Rocquencourt - 2007
 */
#include "sci_mpi.h"
#include "s_mpi_send.h"
#include "build_buff_type.h"

void C2F(s_mpi_send)(int *pack, int *n, double *buff, int *dest, int *tag, int *comm, int *res){
	int errcode, errclass;	/* Error mgmt */
	char error_msg[MPI_MAX_ERROR_STRING];
	int error_msg_length;
	int l;			/* size of the buff to send in bytes */
  
	MPI_Datatype buff_type;

	MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
#ifdef DEBUG
	if (debuglevel >= 5) {    
		int *ptr_int = (int *)buff;
		(void) fprintf(stderr, "MPI_SEND: n : %d\ndest :%d\ntag : %d\ncomm : %d^npack[0] %d\npack[1] %d\nptr_int[0] %d\nptr_int[1] %d\nptr_int[2] %d\nptr_int[3] %d\nbuff[2] %f\nbuff[3] %f\n", 
					   *n, *dest, *tag, *comm, 
					   pack[0], pack[1], 
					   ptr_int[0],ptr_int[1],
					   ptr_int[2],ptr_int[3],
					   buff[2],buff[3]);
		(void) fflush (stderr);	
	}
#endif /* DEBUG */

#ifdef DEBUG
	if (debuglevel >= 10) {
		int i;
		(void) fprintf(stderr, "MPI_SEND: %d|", *n);	
		for (i = 0; i < *n; ++i) {
			(void) fprintf(stderr, "%d:", 
						   pack[i]);
		}
		(void) fprintf(stderr, "\n");
		(void) fflush(stderr);	
	}	
#endif /* DEBUG */
  
	errcode = build_buff_type(*n, pack, &buff_type, &l);
	if (errcode != MPI_SUCCESS){
		MPI_Error_string(errcode, error_msg, &error_msg_length);
		MPI_Error_class(errcode, &errclass);
		(void) fprintf(stderr, "Error mpi_send -build_buff_type-: %d:%s\n", 
					   errclass, error_msg);
		(void) fflush(stderr);
		*res = errcode;
		return;
	}
	errcode = MPI_Type_commit(&buff_type);
	if (errcode != MPI_SUCCESS){
		MPI_Error_string(errcode, error_msg, &error_msg_length);
		MPI_Error_class(errcode, &errclass);
		(void) fprintf(stderr, "Error mpi_send -MPI_Type_commit-: %d:%s\n", 
					   errclass, error_msg);
		(void) fflush(stderr);
		*res = errcode;
		return;
	}
    
	/* Send the packing vector. The recv
	   part must make a call to probe
	   firts to have its size. */
	errcode = MPI_Send(pack, *n, MPI_INT, *dest, *tag, MPI_COMM_WORLD);  
	if (errcode != MPI_SUCCESS){
		MPI_Error_string(errcode, error_msg, &error_msg_length);
		MPI_Error_class(errcode, &errclass);
		(void) fprintf(stderr, "Error mpi_send -MPI_Send-: %d:%s\n", 
					   errclass, error_msg);
		(void) fflush(stderr);
		*res = errcode;
		return;
	}
	/* Send the data... */
	errcode = MPI_Send(buff, 1, buff_type, *dest, *tag, MPI_COMM_WORLD);  
	if (errcode != MPI_SUCCESS){
		MPI_Error_string(errcode, error_msg, &error_msg_length);
		MPI_Error_class(errcode, &errclass);
		(void) fprintf(stderr, "Error mpi_send -MPI_Send-: %d:%s\n", 
					   errclass, error_msg);
		(void) fflush(stderr);
		*res = errcode;
		return;
	}
  
#ifdef DEBUG
	if (debuglevel >= 10) {
		int i;
		char *ptr = (char*) buff;
		(void) fprintf(stderr, "SEND:");	
		for (i = 0; i < l; ++i) {
			(void) fprintf(stderr, "%d:",ptr[i]);
		}
		(void) fprintf(stderr, "\n");	
		(void) fflush (stderr);	
	}	
#endif /* DEBUG */
	errcode = MPI_Type_free(&buff_type);
	if (errcode != MPI_SUCCESS){
		MPI_Error_string(errcode, error_msg, &error_msg_length);
		MPI_Error_class(errcode, &errclass);
		(void) fprintf(stderr, "Error mpi_send -MPI_Type_free-: %d:%s\n", 
					   errclass, error_msg);
		(void) fflush(stderr);
		*res = errcode;
		return;
	}
	*res = MPI_SUCCESS;
} 