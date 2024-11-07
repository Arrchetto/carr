const char* html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-cn">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>距离监测</title>
    <style>
        body { 
            font-family: Arial, sans-serif;
            text-align: center;
            margin: 20px;
        }
        #distance {
            font-size: 24px;
            color: #333;
            margin: 20px 0;
        }
    </style>
    <script>
        function updateDistance() {
            fetch('/distance')
                .then(response => response.text())
                .then(data => {
                    document.getElementById('distance').innerHTML = 
                        '当前距离: ' + data + ' cm';
                });
        }
        // 每500ms更新一次数据
        setInterval(updateDistance, 500);
    </script>
</head>
<body>
    <h1>距离监测系统</h1>
    <div id="distance">加载中...</div>
</body>
</html>
)rawliteral";