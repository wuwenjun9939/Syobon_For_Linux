g++ -o syobon main.cpp loadg.cpp \
    -I ./DxLibMake \
    -D DX_GCC_COMPILE \
    libDxLib.a \
    -lEGL -lGLESv2 -lopenal -lpng -ltiff -lvorbisfile -lvorbis -logg -lopusfile -lopus -ltheora -ltheoradec -lz -lpthread -ldl
