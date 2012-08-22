AccountServerConnectionTest = (function() {
	return {
		connectAndSend: function() {
			var result = 4;
			Assert.areIdentical(5, result);
		},
		connectAndSend2: function() {
			var result = 4;
			Assert.areIdentical(5, result);
		}
	};
})();

Enhance.discoverTests(AccountServerConnectionTest).runTests();