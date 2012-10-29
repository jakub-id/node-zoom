cmd_Release/zoom.node := ln -f "Release/obj.target/zoom.node" "Release/zoom.node" 2>/dev/null || (rm -rf "Release/zoom.node" && cp -af "Release/obj.target/zoom.node" "Release/zoom.node")
