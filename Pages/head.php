<!DOCTYPE html>
<html>
<head>
<title>HEAD request</title>
</head>

<body>
<h2>HEAD request</h2><br />
<?php
stream_context_set_default(
    array(
        'http' => array(
            'method' => 'HEAD'
        )
    )
);
$headers = get_headers('http://google.com');
foreach ($headers as $elem)
	echo $elem . '<br />'
?>
</body>

</html>
