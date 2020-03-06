<!DOCTYPE html>
<html>
	<head>
		<title>GET request</title>
	</head>

	<body>
		<h2>Get request</h2>
		<form action="/dashboard/posted.php" method="get">
			<label for="firstname">First name:</label>
			<input type="text" id="firstname" name="firstname" />

			<label for="favlang">Favorite programming language:</label>
			<select id="favlang">
				<option value="c">C</option>
				<option value="cpp">C++</option>
				<option value="js">JavaScript</option>
				<option value="python">Python</option>
				<option value="haskell">Haskell</option>
			</select><br /><br />

			<input type="submit" value="Submit">
		</form>
	</body>
</html>