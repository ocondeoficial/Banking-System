document.getElementById('loginForm').addEventListener('submit', function(event) {
    event.preventDefault();

    const cpf = document.getElementById('cpf').value;
    const password = document.getElementById('senha').value;

    const loginData = {
        cpf: cpf,
        password: password
    };

    fetch('http://localhost:8080/account/login', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(loginData)
    })
    .then(response => {
        if (!response.ok) {
            return response.json().then(error => { throw error });
        }
        return response.json();
    })
    .then(data => {
        if (data.success) {
            sessionStorage.setItem('holder', data.holder);
            sessionStorage.setItem('balance', data.balance);
            sessionStorage.setItem('cpf', data.cpf);

            window.location.href = 'perfil.html';
        } else {
            alert('Erro ao fazer login: ' + data.message);
        }
    })
    .catch(error => {
        console.error('Erro:', error);
        alert('Erro ao tentar fazer login.');
    });
});
