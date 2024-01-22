<?php
// Conecta com o banco de dados
$pdo = new PDO("mysql:host=localhost;dbname=chat", 'root', 'admin') or print("Error");
$table = "";

if (isset($_GET['chat'])) {
    $table = $_GET['from'] . '-' . $_GET['to'];
    $query = "CREATE TABLE IF NOT EXISTS `$table` (id INTEGER PRIMARY KEY AUTO_INCREMENT, user VARCHAR(30), msg TEXT)";
    $pdo->query($query);
    $query = "INSERT IGNORE INTO `chats` (chat) VALUES ('$table')";
    $pdo->query($query);
}

if (isset($_GET['message'])) {
    $date = new DateTime();
    $name = $_GET['name'];
    $message = $_GET['message'];
    $last_update = $date->format('Y-m-d H-i-s');

    $query = "INSERT INTO `$table` (user,msg) VALUES ('$name','$message')";
    $pdo->query($query);
    $query = "UPDATE `chats` SET `last_update` = '$last_update' WHERE `chat` = '$table'";
    $pdo->query($query);

    die;
}

// Atualiza a exibiçaõ das mensagens na tela
if (isset($_GET['messages'])) {
    $messages = $_GET['messages'];
    $new = 0;
    $last_message = "";
    $query = "SELECT * FROM `$table`";
    $result = $pdo->query($query)->fetchAll();

    foreach ($result as $registro) {
        $new++;
        $last_message = '<div class="received"><span class="block"><span class="user">' . $registro['user'] . '</span><br><span class="message">'. $registro['msg'] . '</span></span></div>';
    }

    if ($new == $messages) {
        $messages = "Não há novas mensagens";
    } else {
        $messages = $last_message;
    }
    
    echo $messages;
    die;
}

?>

<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CryptoChat</title>
    <link rel="stylesheet" href="style.css">
    <link rel="stylesheet" href="animation.css">
    <link rel="stylesheet" href="../themes/dark.css" id="themes">
</head>
<body onload="getName()">
    <div id="screen">
        <header>
            <a href="../no-chat.html"><img src="../../icons/close.svg" alt="close-button" class="icon"></a>
            <h1><?php echo $table ?></h1>
            <img src="../../icons/dots-vertical.svg" alt="option" class="icon" onclick="open_menu()">
            <ul id="options">
                <li onclick="changeTheme()">Mudar tema</li>
            </ul>
        </header>

        <main></main>

        <footer id="actions">
            <input type="text" name="message" id="message" placeholder="Menssagem" autocomplete="off">
            <input type="submit" id="send" value="" onclick="sendMessage()">
        </footer>
    </div>
    
    <script src="../scripts/rsa.js"></script>
    <script src="../scripts/main.js"></script>
    <script>
        setTimeout(() => {
            updateMessages();
        }, 5000);

        setInterval(() => {
            updateMessages();
        }, 3000);
    </script>
</body>
</html>