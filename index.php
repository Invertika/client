<?php
//echo $_POST["username"];
//echo $_POST["password"];
?>

<!DOCTYPE html>
<html>
    <head>
        <title>Invertika</title>
        <script type="text/javascript" src="libs/blackbird/blackbird.js"></script>
        <script type="text/javascript" src="libs/jquery-1.7.2.min.js"></script>
        <link type="text/css" rel="Stylesheet" href="libs/blackbird/blackbird.css" />
        <link rel="stylesheet" type="text/css" href="style.css" />
    </head>
    <body bgcolor="white">
        <div id="info" align="center">    
            <!--
            <font style ="font-size: 10px; font-family: Courier New">  
            <span id="framecounter">(0/0 fps)</span>
            </font>  
            -->
        </div>
        <div id="jsapp" align="center">
            <script type="text/javascript" src="libs/socket.io.js"></script>
            <script type="text/javascript" src="libs/melonJS-0.9.3-min.js"></script>
            <!--<script type="text/javascript" src="main.js"></script>-->
            <script type="text/javascript" src="index.js"></script>
            
            <script type="text/javascript">
                init('<?php echo $_POST["username"]?>', '<?php echo $_POST["password"]?>');
            </script>
        </div>
        <div id="footer" align="center">
        </div>
    </body>
</html>
