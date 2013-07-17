
#foreach(tut access copy move erase find custom_id dense_data key_dispatch default_data_type)
foreach(tut relaunch)
   add_executable(${tut} ${tut}.cpp)
endforeach()


