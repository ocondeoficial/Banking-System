document.getElementById('registerForm').addEventListener('submit', async function(event) {
    event.preventDefault();

    const name = document.getElementById('name').value.trim();
    const cpf = document.getElementById('cpf').value.trim();
    const senha = document.getElementById('senha').value.trim();

    if (!name || !cpf || !senha || cpf.length !== 11) {
        alert('Por favor, preencha todos os campos corretamente.');
        return;
    }

    const payload = {
        holder: name,
        cpf: cpf,
        password: senha,
        initial_balance: 0.0
    };

    try {
        const response = await fetch('http://localhost:8080/account/create', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(payload)
        });

        if (response.ok) {
            document.getElementById('successMessage').style.display = 'block';
            document.getElementById('errorMessage').style.display = 'none';
            document.getElementById('registerForm').reset();
        } else {
            throw new Error('Erro na criação da conta.');
        }
    } catch (error) {
        document.getElementById('successMessage').style.display = 'none';
        document.getElementById('errorMessage').style.display = 'block';
    }
});