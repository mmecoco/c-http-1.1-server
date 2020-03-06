<!DOCTYPE html>
<html>
	<head>
		<title>POST request POSTED</title>
	</head>

	<body>
		Thanks for your submission
		<?php
		if (empty($_POST["firstname"])) {
			if (empty($_GET["firstname"])) {
				echo "stranger";
			} else {
				echo $_GET["firstname"];
			}
		} else {
			echo $_POST["firstname"];
		}
		?>.
	</body>
</html>