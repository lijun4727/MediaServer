#boost库安装(保证提前安装python2)
#./bootstrap.sh --with-python=python2
#sudo ./b2 install

#Debug建立cmake -DCMAKE_BUILD_TYPE=Debug ..

# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.0)

# 项目信息
project (media_server)


include_directories("../common")

# 查找当前目录下的所有源文件
aux_source_directory(. SELF_SRCS)
aux_source_directory(../common COMMON_SRCS)

# 指定生成目标
add_executable(media_server ${SELF_SRCS} ${COMMON_SRCS})

target_link_libraries(media_server pthread boost_thread)