document.addEventListener("DOMContentLoaded", function() {
    // Recupera todas as transações do sessionStorage
    const transactions = JSON.parse(sessionStorage.getItem("transactions")) || [];
  
    // Verifica se há transações armazenadas
    if (transactions.length === 0) {
      document.getElementById("extratoContainer").innerHTML = "Nenhuma transação encontrada.";
    } else {
      // Ordena as transações por data (mais recente primeiro)
      transactions.sort((a, b) => new Date(b.date) - new Date(a.date));
  
      // Exibe as transações na página
      transactions.forEach(operation => {
        const container = document.createElement("div");
        container.classList.add("extrato-item");
  
        const operationName = document.createElement("p");
        operationName.textContent = `Operação: ${operation.operation}`;
  
        const amount = document.createElement("p");
        amount.textContent = `Valor: R$ ${parseFloat(operation.amount).toFixed(2)}`;
  

        const date = document.createElement("p");
        date.textContent = `Data: ${new Date(operation.date).toLocaleString()}`;
  
        container.appendChild(operationName);
                // Exibe o destinatário para transferências abaixo do valor
                if (operation.operation === "Transferência") {
                    const toCpf = document.createElement("p");
                    toCpf.textContent = `Destinatário: CPF ${operation.to_cpf}`;
                    container.appendChild(amount);
                    container.appendChild(toCpf);  
                     
                  } else {
                    container.appendChild(amount);
                  }
            
        container.appendChild(date);
  
        // Adiciona o container na página
        document.getElementById("extratoContainer").appendChild(container);
      });
    }
  });
  function cancelarOperacao() {
    window.location.href = "perfil.html";
  }
  
