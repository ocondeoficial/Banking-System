document.getElementById("depositForm").addEventListener("submit", function(event) {
  event.preventDefault();

  const cpf = sessionStorage.getItem("cpf");
  const amount = parseFloat(document.getElementById("valorDeposito").value);

  if (isNaN(amount) || amount <= 0) {
    document.getElementById("message").textContent = "Valor inválido para depósito.";
    return;
  }

  const data = {
    cpf: cpf,
    amount: amount
  };

  fetch("http://localhost:8080/account/deposit", {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify(data)
  })
  .then(response => {
    if (response.ok) {
      return "Seu depósito foi um sucesso!!";
    } else {
      throw new Error("Depósito falhou!");
    }
  })
  .then(responseText => {
    document.getElementById("message").textContent = responseText;

    // Recupera todas as transações ou cria um novo array se não existir
    let transactions = JSON.parse(sessionStorage.getItem("transactions")) || [];

    // Adiciona a nova operação de depósito ao array
    transactions.push({ operation: "Depósito", amount: amount, date: new Date() });

    // Armazena o array atualizado no sessionStorage
    sessionStorage.setItem("transactions", JSON.stringify(transactions));

  })
  .catch(error => {
    document.getElementById("message").textContent = error.message;
  });
});


function cancelarOperacao() {
  window.location.href = "perfil.html";
}
