{
  "targets": [
    {
      "cflags": ["<!@(yaz-config --libs)"],
      "libraries": ["<!@(yaz-config --libs)"],
      "target_name": "zoom",
      "sources": [ "src/zoom.cc", "src/scanset.cc", "src/connection.cc" ]
    }
  ]
}
