# Nome do projeto
PROJECT_NAME = ImageProcessingGUI

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Fonte e objetos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Flags de compilação
CXX = g++
CXXFLAGS = -Wall -Wextra -I$(INC_DIR) `wx-config --cxxflags`
LDFLAGS = `wx-config --libs`

# Regra principal
all: $(PROJECT_NAME)

# Regra para criar o executável
$(PROJECT_NAME): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Regra para compilar os arquivos .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	rm -rf $(OBJ_DIR) $(PROJECT_NAME)

.PHONY: all clean
