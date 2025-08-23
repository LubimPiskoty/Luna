# Luna

## Command for converting to video
ffmpeg -framerate 24 -i render-%d.png -preset veryslow -qp 0 -vcodec mpeg4 output.mp4