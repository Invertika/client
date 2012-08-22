AccountServerConnectionTest = (function() {
	return {
		connect: function() {
			var connection=new AccountServerConnection("echo.websocket.org", 80);
			connection.login("test", "test");
			
			//Schl�gt fehl wenn das Socket null ist
			Assert.areNotIdentical(connection.socket, null);
		},
		
		checkParameters: function() {
			var connection=new AccountServerConnection("echo.websocket.org", 80);
			connection.login("test", "secret");
			
			//Pr�ft ob die Parameter korrekt an die Klasse �bergeben wurden
			Assert.areIdentical(connection.username, "test");
			Assert.areIdentical(connection.password, "secret");
			Assert.areIdentical(connection.ip, "echo.websocket.org");
			Assert.areIdentical(connection.port, 80);
		}
	};
})();

Enhance.discoverTests(AccountServerConnectionTest).runTests();