<?php

$pdo = new PDO("mysql:host=localhost;dbname=chat", 'root', 'admin') or print("Error");

if (isset($_GET['chat'])) {
    $table = $_GET['chat'];
    $query = "SELECT * FROM `$table`";
    $result = $pdo->query($query);
    $content = "<table>";

    foreach ($result as $registro) {
        $content .= '<tr><td>' . $registro['user'] . '</td><td>' . $registro['msg'] . '</td></tr>';
    }

    $content .= "</table>";
    if (isset($_GET['update'])) {
        echo $content;
        die;
    }
}

?>

<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Chat</title>
    <link rel="stylesheet" href="themes/dark.css">
    <style>
        * {
            margin: 0;
            padding: 0;
        }
        body {
            background: var(--background);
            font-family: var(--font);
            color: var(--secondary-text);
        }
        header {
            background: var(--tertiary-color);
            padding: 15px;
            color: var(--primary-text);
        }
        main {
            display: flex;
            justify-content: center;
            padding-top: 40px;
        }
        table { 
            width: 80%;
        }
        td {
            padding: 5px;
            border-bottom: 1px solid var(--secondary-text);
            border-left: 1px solid var(--secondary-text);
        }
    </style>
</head>
<body>
    <header>
        <h1><?php echo $table ?></h1>
    </header>

    <main></main>

    <script>
        setInterval(() => {
            let uri = document.location.href + "&update=true";
            fetch(uri)
            .then((response) => {
                return response.text();
            })
            .then((content)=>{
                document.querySelector("main").innerHTML = content;
            })
        }, 1000);
    </script>
</body>
</html>