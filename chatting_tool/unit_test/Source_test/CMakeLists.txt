add_executable(ChatServerTest ChatServerTest.cpp /home/vany/training_assignment_2/chatting_tool/source/ChatServer.c)
target_link_libraries(ChatServerTest gtest pthread)