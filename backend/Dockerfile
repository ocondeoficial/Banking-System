# Imagem base
FROM gcc:latest

# Instalar dependências
RUN apt-get update && apt-get install -y libpq-dev cmake

# Copiar os arquivos do projeto
COPY . /app
WORKDIR /app

# Build do projeto
RUN mkdir build && cd build && cmake .. && make

# Executar o backend
CMD ["./build/BankingSystem"]
