/*
    Responsável por Encriptar e decriptar as mensagens de uma conversa
*/
/*
    ========= Criptografia RSA ===========
    GERAR CHAVES
    1. Escolhe um primo (p)
    2. Escolhe um primo (q)
    3. Multiplica p por q (n)
    4. Calcula o phi de n (p - 1)*(q - 1)
    5. Gera um primo e | 1 < e < phi(n) e mdc(phi(n), e) == 1
    6. Gera um d | d*e mod(phi(n)) == 1

    ENCRIPTAR
    - code^e (mod n)
    
    DECRIPTAR
    - code^d (mod n)
*/

function mdc(n1, n2) {
    let a, b, resto;
    a = n1;
    b = n2;
    do {
        resto = a % b;
        a = b;
        b = resto;
    } while (resto != 0);

    return a;
}

function isPrime(n) {
    if (n <= 1) return false;
    if (n <= 3) return true;

    if (n % 2 == 0 || n % 3 == 0) return false;

    for (let i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

function keyGenerate() {
    let p, q, n, phi, e, d;

    do {
        p = Math.round(Math.random() * 10000);
    } while (!isPrime(parseInt(p)));

    do {
        q = Math.round(Math.random() * 10000);
    } while (q == p || !isPrime(parseInt(q)));

    n = p * q;
    phi = (p - 1) * (q - 1);

    do {
        e = Math.round(Math.random() * phi);
    } while (mdc(phi, e) !== 1);
    
    /* MENOR EFICIÊNCIA
    do {
        d = Math.round(Math.random() * phi);
    } while ((d*e-1) % phi != 0);
    */
    
    d = modInverse(e, phi);

    publicKey = [n, e];
    privateKey = [d, n];

    sessionStorage.setItem("public_key", publicKey[0] + " " + publicKey[1]);
    sessionStorage.setItem("private_key", privateKey[0] + " " + privateKey[1]);
}

function modInverse(a, m) {
    let m0 = m;
    let y = 0;
    let x = 1;

    if (m === 1) return 0;

    while (a > 1) {
        let q = Math.floor(a / m);
        let t = m;

        m = a % m;
        a = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0) x += m0;

    return x;
}

function encrypt(message) {
    let key = sessionStorage.getItem("public_key");
    let n = key.split(" ")[0];
    let e = key.split(" ")[1];
    let encrypted = [];

    for (let i = 0; i < message.length; i++) {
        let charCode = message.charCodeAt(i);
        let encryptedCode = modPow(charCode, e, n);
        encrypted.push(encryptedCode);
    }

    return encrypted.join(" ");
}

function decrypt(encryptedMessage) {
    let message = encryptedMessage.split(" ");
    let key = sessionStorage.getItem("private_key");
    let d = key.split(" ")[0];
    let n = key.split(" ")[1];
    let decrypted = [];

    for (let i = 0; i < message.length; i++) {
        let decryptedCode = modPow(message[i], d, n);
        let char = String.fromCharCode(decryptedCode);
        decrypted.push(char);
    }

    return decrypted.join('');
}

function modPow(base, exponent, modulus) {
    if (modulus === 1) return 0;
    let result = 1;
    base = base % modulus;

    while (exponent > 0) {
        if (exponent % 2 === 1) {
            result = (result * base) % modulus;
        }
        exponent = Math.floor(exponent / 2);
        base = (base * base) % modulus;
    }

    return result;
}
