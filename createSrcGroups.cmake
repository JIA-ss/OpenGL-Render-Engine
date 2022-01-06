function (createSrcGroups FILE_LIST )
  # we want to get the relative path from the
  # current source dir
  string(LENGTH ${CMAKE_CURRENT_SOURCE_DIR} curDirLen)
  set(TMP_FILE_LIST ${${FILE_LIST}})

  foreach ( SOURCE ${TMP_FILE_LIST} )
    string(LENGTH ${SOURCE} fullPathLen)
    math(EXPR RelPathLen ${fullPathLen}-${curDirLen})
    string(SUBSTRING ${SOURCE} ${curDirLen} ${RelPathLen} curStr)

    string ( REGEX REPLACE "[\\/]" "\\\\" normPath ${curStr} )
    string ( REGEX MATCH "\\\\(.*)\\\\" ouput ${normPath} )
    if(NOT CMAKE_MATCH_1 STREQUAL "")
      source_group ( ${CMAKE_MATCH_1} FILES ${SOURCE} )
    endif()
  endforeach()
endfunction()