OPTS = -Wall

firstOpenGlApp: main.cpp ModelData.o Shader.o ModelHandler.o Simulation.o
	g++ ${OPTS} main.cpp -o firstOpenGlApp Simulation.o `pkg-config --libs --cflags opencv` ModelData.o Shader.o ModelHandler.o -lglut -lGLU -lGL -lGLEW -lassimp 

Shader.o: Shader.cpp Shader.hpp
	g++ ${OPTS} -c Shader.cpp -lglut -lGLU -lGL -lGLEW -lassimp

ModelData.o: ModelData.cpp ModelData.hpp Shader.cpp Shader.hpp
	g++ ${OPTS} -c ModelData.cpp -lglut -lGLU -lGL -lGLEW -lassimp

ModelHandler.o: ModelHandler.cpp ModelHandler.hpp Shader.hpp Shader.cpp ModelData.hpp ModelData.cpp
	g++ ${OPTS} -c ModelHandler.cpp Shader.cpp ModelData.cpp -lglut -lGLU -lGL -lGLEW -lassimp

Simulation.o: Simulation.cpp Simulation.hpp ModelHandler.cpp ModelHandler.hpp Shader.hpp Shader.cpp ModelData.hpp ModelData.cpp
	g++ ${OPTS} -c Simulation.cpp `pkg-config --libs --cflags opencv` ModelHandler.cpp Shader.cpp ModelData.cpp -lglut -lGLU -lGL -lGLEW -lassimp

clean:
	rm -f *.o

run:
	make clean
	make firstOpenGlApp
	./firstOpenGlApp

run100:
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp
	./firstOpenGlApp