# Understand User Push-button

## Extension connectors
According to USer MAnual page 22
PA0 is used for USER-BUTTON

So we have to enable GPIOA

No other information from User_Manual or Reference_Manual or even in STM32F3 
discovery Datasheet.

### Idea N1
User button off = input data register PA0 == 0
User button on = input data register PA0 == 1

Set up GPIOA0 to input and check GPIOA->IDR 0
