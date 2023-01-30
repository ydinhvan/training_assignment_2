extern "C"{
    #include "include.h"
}
#include <gtest/gtest.h>
#include "/home/vany/training_assignment_2/chatting_tool/unit_test/include_test/fff.h"


DEFINE_FFF_GLOBALS;

/*Chat Server*/

FAKE_VOID_FUNC(accept_client, int, fd_set*, int*, int, SA_in*);
FAKE_VOID_FUNC(process_client, int, fd_set*, int, int);
FAKE_VOID_FUNC(log_activity, char*);

class ServerTest: public testing::Test
{
 public:
    void SetUp()
	{

	    RESET_FAKE(accept_client)
	    RESET_FAKE(process_client)
	    RESET_FAKE(log_activity)

		FFF_RESET_HISTORY();

	}

};


TEST_F(ServerTest, run_serverTest)
{
    run_server(int* serv_fd, fd_set* cli_fds, fd_set* main_fd, int* max_fd, SA_in* cli_addr);
    ASSERT_EQ(accept_client_fake.call_count, 1);
}

