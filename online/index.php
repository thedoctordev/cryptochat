<?php
// Conecta com o banco de dados
$pdo = new PDO("mysql:host=localhost;dbname=chat", 'root', 'admin') or print("Error");

// Registra o usuário
if (isset($_GET['start'])) {
    $name = $_GET['name'];
    $date = new DateTime();
    $lastView = $date->format('Y-m-d H-i-s');
    $key = $_GET['key'];
    $query = "INSERT INTO `users` (`id`, `name`, `last_view`, `key`, `call`) VALUES (NULL, '$name', '$lastView', '$key', NULL)";
    $pdo->query($query);
}

// Verifica se o usuário ainda está conectado ao servidor
if (isset($_GET['online'])) {
    $name = $_GET['name'];
    $date = new DateTime();
    $lastView = $date->format('Y-m-d H-i-s');
    $query = "UPDATE `users` SET `last_view` = '$lastView' WHERE `name` = '$name'";
    $pdo->query($query);
}

// Exclui os usúarios inativos a mais de um minutos e lista os que ainda estão online
if (isset($_GET['update'])) {
    $query = "SELECT * FROM `users`";
    $result = $pdo->query($query)->fetchAll();
    
    $list = "<section id='users'><table><caption>Usuários online</caption>";
    foreach($result as $registro) {
        $date1 = new DateTime($registro['last_view']);
        $date2 = new DateTime();
        $diference = $date1->diff($date2);
        $diferenceImMinutes = $diference->i + ($diference->h + $diference->days * 24) * 60;

        if ($diferenceImMinutes > 1) {
            $query = 'DELETE FROM `users` WHERE `users`.`id` = ' . $registro['id'];
            $pdo->query($query);
        } else {
            $list .= '<tr><td><button id="' . $registro['name'] . '" onclick="call(\'' . $registro['name'] . '\')"></button></td><td>' . $registro['name'] . '</td></tr>';
        }
    }
    
    $list .= "</table></section>";

    $query = "SELECT * FROM `chats`";
    $result = $pdo->query($query)->fetchAll();
    
    $list .= "<section id='chats'><table><caption>Conversas abertas</caption>";
    foreach($result as $registro) {
        $date1 = new DateTime($registro['last_update']);
        $date2 = new DateTime();
        $diference = $date1->diff($date2);
        $diferenceImMinutes = $diference->i + ($diference->h + $diference->days * 24) * 60;

        if ($diferenceImMinutes > 50) {
            $query = 'DROP TABLE `chat`.`' . $registro['chat'] . '`';
            $pdo->query($query);
            $query = 'DELETE FROM `chats` WHERE `chats`.`chat` = \'' . $registro['chat'] . '\'';
            $pdo->query($query);
        } else {
            $list .= '<tr><td><a target="_blank" href="chat.php?chat=' . $registro['chat'] . '">' . $registro['chat'] . '</a></td></tr>';
        }
    }
    $list .= "</table></section>";
    
    echo $list;
    die;
}   

// Chama outro usuário para uma conversa
if (isset($_GET['call'])) {
    $call = $_GET['call'];
    $target = $_GET['target'];
    $query = "UPDATE `users` SET `call` = '$call' WHERE `name` = '$target'";
    $pdo->query($query);
}

// Coleta a chave pública para do outro usuário
if (isset($_GET['getKey'])) {
    $name = $_GET['getKey'];
    $query = 'SELECT * FROM `users`';
    $result = $pdo->query($query)->fetchAll();
    $key = "";
    foreach ($result as $registro) {
        if ($registro['name'] == $name) {
            $key = $registro['key'];
        }
    }

    echo $key;
    die;
}

// Verifica se exite alguma notiicação (call)
if (isset($_GET['verify'])) {
    $name = $_GET['name'];
    $query = "SELECT * FROM `users`";
    $result = $pdo->query($query)->fetchAll();
    $notification = "";
    foreach ($result as $registro) {
        if ($registro['name'] == $name && $registro['call'] != NULL) {
            $notification .= '<span onclick="chat(\'' . $registro['call'] . '\',\'' . $name . '\')">' . $registro['call'] . '</span>';
        }
    }

    echo $notification;
    die;
}

?>

<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CryptoChat - Dashboard</title>
    <link rel="stylesheet" href="themes/dark.css" id="themes">
    <link rel="stylesheet" href="style.css">
</head>
<body onload="load()">
    <header>
        <div id="notifications"></div>
        <h1>CryptoChat</h1>
        <div id="alert"></div>
    </header>

    <main>
    </main>
    
    <aside>
        <iframe src="no-chat.html" frameborder="0" id="chat"></iframe>
        <img src="../icons/close.svg" alt="close" id="close_frame" onclick="closeFrame()">
    </aside>

    <script src="scripts/rsa.js"></script>
    <script src="scripts/main.js"></script>
    <script>
        dashboard();
        setInterval(() => {
            dashboard();
            verify();
        }, 3000);
    </script>
</body>
</html>