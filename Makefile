all:
	@echo "Generating Test Data"
	@echo "===================="
	@cd BitmapWorkloadGenerator && \
		make # Generate the test data
	@echo ""
	@echo "Running BitmapEngine"
	@echo "===================="
	@cd BitmapEngine && \
		make # Run the BitmapEngine

# Run clean in both the `BitmapEngine` and `BitmapWorkloadGenerator`
clean:
	@echo "Cleaning Test Data"
	@echo "=================="
	@cd BitmapWorkloadGenerator && \
		make clean
	@echo ""
	@echo "Cleaning BitmapEngine"
	@echo "====================="
	@cd BitmapEngine && \
		make clean
