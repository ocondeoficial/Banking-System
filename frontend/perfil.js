const holder = sessionStorage.getItem("holder");
const balance = sessionStorage.getItem("balance");
const cpf = sessionStorage.getItem("cpf");

if (!holder || !balance || !cpf) {
  alert('Dados do usuário não encontrados. Por favor, faça login novamente.');
  window.location.href = 'login.html';
} else {

  document.getElementById("holder").textContent = holder;
  document.getElementById("balance").textContent = "R$ " + parseFloat(balance).toFixed(2);
  document.getElementById("cpf").textContent = cpf;
}

function updateBalance(newBalance) {
  if (newBalance >= 0) {
    sessionStorage.setItem("balance", newBalance.toFixed(2));
    document.getElementById("balance").textContent = "R$ " + newBalance.toFixed(2);
  } else {
    alert("Saldo não pode ser negativo.");
  }
}

function fetchBalance() {
  const cpf = sessionStorage.getItem("cpf");
  
  if (cpf) {
    fetch(`http://localhost:8080/account/find?cpf=${cpf}`)
      .then(response => {
        if (!response.ok) {
          throw new Error('Erro ao recuperar os dados do servidor');
        }
        return response.json();
      })
      .then(data => {
        if (data && data.balance !== undefined) {
          updateBalance(data.balance);
        } else {
          alert("Erro ao recuperar o saldo.");
        }
      })
      .catch(error => {
        console.error("Erro na requisição:", error);
        alert("Erro ao recuperar o saldo.");
      });
  } else {
    alert("CPF não encontrado no sessionStorage.");
  }
}

updateBalance(parseFloat(balance));

setInterval(fetchBalance, 100);

function redirectToTransfer() {
  window.location.href = "transferir.html";
}

function redirecionarSaque() {
  window.location.href = "saque.html";
}

function redirecionarDeposito() {
  window.location.href = "deposito.html";
}

function redirecionarExtrato() {
  window.location.href = "extrato.html";
}
