document.getElementById("valorForm").addEventListener("submit", function(event) {
    event.preventDefault();

    const destinatario = document.getElementById("destinatario").value;
    const valor = parseFloat(document.getElementById("valorTransferencia").value);
    const cpf = sessionStorage.getItem("cpf");

    if (isNaN(valor) || valor <= 0) {
        document.getElementById("message").textContent = "Valor inválido para transferência.";
        return;
    }

    if (!destinatario) {
        document.getElementById("message").textContent = "CPF do destinatário é obrigatório.";
        return;
    }

    const data = {
        from_cpf: cpf,
        to_cpf: destinatario,
        amount: valor
    };

    fetch("http://localhost:8080/account/transfer", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(data)
    })
    .then(response => {
        if (response.ok) {
            return "Sua transferência foi um sucesso!!";
        } else {
            throw new Error("Transferência falhou!");
        }
    })
    .then(responseText => {
        document.getElementById("message").textContent = responseText || "Transferência realizada com sucesso!";

        // Se a transferência foi bem-sucedida, armazena a transação
        let transactions = JSON.parse(sessionStorage.getItem("transactions")) || [];

        transactions.push({
            operation: "Transferência",
            amount: valor,
            to_cpf: destinatario,
            date: new Date()
        });

        sessionStorage.setItem("transactions", JSON.stringify(transactions));

    })
    .catch(error => {
        // Se ocorrer um erro, exibe a mensagem e não redireciona
        document.getElementById("message").textContent = error.message;
    });
});

function cancelarOperacao() {
    window.location.href = "perfil.html";
}
