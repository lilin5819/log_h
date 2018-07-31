#include "log.h"

LOG_DEF()

int main(int argc, char const *argv[])
{
    char *null_ptr = NULL;
    char *myname = "lilin";
    char *empty_str = "";
    log_d(argc);
    log_d(strlen(argv[0]));

    set_log_app((char*)argv[0]);

    log_s(myname);

    set_log_file("/tmp/test.log");
    log_p(myname);

    log_p(null_ptr);
    log_s(null_ptr);
    log_s(empty_str);
    
    set_log_mode(_MODE_VERBOSE);

    log_e("This is an ERROR msg !!!!\n");
    ok( 2+1 == 2);

    ok(null_ptr != NULL);
    ok(myname != NULL);

    set_log_mode(0);

    logs("%s can't catch this msg in %s , but in log file %s!!!\n","You","terminal","/tmp/test.log");

    set_log_mode(_MODE_VERBOSE|_MODE_NO_LINE);

    logs("test no line code\n");
    
    set_log_mode(_MODE_VERBOSE);

    log_mem(myname,strlen(myname)+1);

    logs("Thank %s\n","you");

    return 0;
}
