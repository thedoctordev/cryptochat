const inputMessage = (document.getElementById("message")) ? document.getElementById("message") : null;

let menu = false;
let name;

// Chamada ao carregar a página, solicita o nome do usuário
function load() {
    getName();
    keyGenerate();
    startConection();
}

function getName() {
    if (!sessionStorage.getItem("user")) {
        sessionStorage.setItem("user", prompt("Digite o nome que deseja usar:"));
    }
    name = sessionStorage.getItem("user");
}

// Abre/Fecha o menu de opções do canto superior direito
function open_menu() {
    let menu_element = document.getElementById("options");
    if (menu) {
        menu_element.className = "close-menu";
        setTimeout(() => { menu_element.style.display = "none" }, 100);
    } else {
        menu_element.className = "open-menu";
        menu_element.style.display = "block";
    }
    menu = !menu;
}

// Muda o tema de cores
function changeTheme() {
    const theme = document.getElementById("themes");
    if (theme.href.includes('light')) {
        theme.href = "../themes/dark.css";
    } else {
        theme.href = "../themes/light.css";
    }
    open_menu();
}

// Função que  envia uma mensagem
function sendMessage() {
    if (!inputMessage.value) {
        return;
    }

    let main = document.querySelector("main");
    main.innerHTML += `<div class="sent"><span class="block"><span class="user">Você</span><br><span class="message">${inputMessage.value}</span></span></div>`;
    main.scrollTo(0, main.scrollHeight);

    let message = encrypt(inputMessage.value);
    let chat = sessionStorage.getItem("chat");
    let uri = `index.php?${chat}&name=${name}&message=${message}`;
    
    fetch(uri)
    .then(() => {
        inputMessage.value = "";
        inputMessage.focus();
        updateMessages();
    })
    .catch((error) => {
        console.error("Erro: ", error);
    })
}

// Exibe as mensagens na página de mensagens
function updateMessages() {
    const main = document.querySelector("main");
    let chat = sessionStorage.getItem("chat");
    let total_messages = document.querySelectorAll(".message").length;
    let uri = `index.php?${chat}&messages=${total_messages}`;

    fetch(uri)
        .then((response) => {
            return response.text();
        })
        .then((messages) => {
            if (messages != "Não há novas mensagens") {
                main.innerHTML += messages;
                let msg = document.querySelectorAll(".message")[document.querySelectorAll(".message").length-1];
                msg.innerText = decrypt(msg.innerText);
                document.querySelector("main").scrollTo(0, document.querySelector("main").scrollHeight);
            }
        })
        .catch((error) => {
            console.error("Erro: ", error);
        });
}

// Exclui os usúarios inativos a mais de um minutos e lista os que ainda estão online
function dashboard() {
    const main = document.querySelector("main");

    fetch('index.php?update=true')
        .then((response) => {
            return response.text();
        })
        .then((data) => {
            main.innerHTML = data;
            document.getElementById(name).onclick = "";
            document.getElementById(name).parentElement.style.opacity = '0';
        })
        .catch((error) => {
            console.error("Erro: ", error);
        })
}

// Registra o usuário
function startConection() {
    let key = sessionStorage.getItem("public_key");
    sessionStorage.removeItem("public_key");
    let uri = `index.php?start=true&name=${name}&key=${key}`;
    fetch(uri)
        .then((response) => {
            return response.text();
        })
        .catch((error) => {
            console.error("Erro: ", error);
        })
}

// Verifica se o usuário ainda está conectado ao servidor
function online() {
    let uri = `index.php?online=true&name=${name}`;
    fetch(uri)
        .then((response) => {
            return response.text();
        })
        .catch((error) => {
            console.error("Erro: ", error);
        })
}

// Chamada ao iniciar uma conversa, solicita que seja criada uma nova tabela de mensagens
function chat(from,to) {
    sessionStorage.setItem("chat", `chat=true&from=${from}&to=${to}`);
    document.getElementById("chat").src = `chat/index.php?chat=true&from=${from}&to=${to}`;
    if (navigator.userAgent.includes("Mobile")) {
        document.getElementById("close_frame").style.display = "block";
        document.getElementById("chat").parentElement.style.width = "100%";
        document.getElementById("chat").parentElement.style.height = "100%";
    }
    getKey(from);
    document.getElementById("notifications"). innerHTML = "";
}

// Envia uma notificação para ouuto usuário informando uma chamada para uma conversa
function call(target) {
    sessionStorage.setItem("chat", `chat=true&from=${name}&to=${target}`);
    let uri = `index.php?call=${name}&target=${target}`;
    fetch(uri)
    .then((response) => {
        return response.text();
    })
    .catch((error) => {
        console.log("Erro: ", error);
    });
    document.getElementById("chat").src = `chat/index.php?chat=true&from=${name}&to=${target}`;
    if (navigator.userAgent.includes("Mobile")) {
        document.getElementById("close_frame").style.display = "block";
        document.getElementById("chat").parentElement.style.width = "100%";
        document.getElementById("chat").parentElement.style.height = "100%";
    }
    getKey(target);
}

function closeFrame() {
    document.getElementById("chat").src = "no-chat.html";
    document.getElementById("close_frame").style.display = "none";
    document.getElementById("chat").parentElement.style.width = "0";
    document.getElementById("chat").parentElement.style.height = "0";
}

function getKey(user) {
    let uri = `index.php?getKey=${user}`;
    fetch(uri)
    .then((response) => {
        return response.text();
    })
    .then((key) => {
        sessionStorage.setItem("public_key", key);
    })
    .catch((error) => {
        console.log("Erro: ", error);
    });
}

// Verifica se existe alguma notificação e exibe se hover alguma
function verify() {
    const element = document.getElementById("notifications");
    let uri = `index.php?verify=true&name=${name}`;

    fetch(uri)
        .then((response) => {
            return response.text();
        })
        .then((notification) => {
            element.innerHTML = notification;
        })
        .catch((error) => {
            console.error("Erro: ", error);
        })
}


// Chamada periódica a cada 3seg para informar que o usuário permanece conectado (online)
setInterval(() => {
    online();
}, 3000);