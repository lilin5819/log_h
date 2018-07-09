#include "log.h"

LOG_DEF()

int main(int argc, char const *argv[])
{
    char *null_ptr = NULL;
    char *myname = "lilin";
    char *empty_str = "";
    log_d(argc);
    log_d(strlen(argv[0]));

    set_log_name((char*)argv[0]);

    log_s(myname);

    set_log_file("/tmp/test.log");
    log_p(myname);

    log_p(null_ptr);
    log_s(null_ptr);
    log_s(empty_str);
    log_e("This is an ERROR msg !!!!");

    ok(null_ptr != NULL);
    ok(myname != NULL);

    set_log_mode(0);
    logs("%s can't catch this msg in %s , but in log file %s!!!","You","terminal","/tmp/test.log");
    set_log_mode(1);


    logs("Thank %s","you");
    return 0;
}
