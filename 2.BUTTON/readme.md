# Understand User Push-button

## Extension connectors
According to User Manual page 22
PA0 is used for USER-BUTTON

So we have to enable GPIOA

No other information from User_Manual or Reference_Manual or even in STM32F3 
discovery Datasheet.

### Idea N1
A nice video from BuildYourCNC explaining how to use a user button:
https://www.youtube.com/watch?v=tzqg03vAdsg&list=PL6PplMTH29SHgRPDufZhfMRoFwRAIrzOp&index=10

In our example we use a different software:
- When user push the button it switch on/off the leds.

Now how to code this:

We just got 1 information and it4s enought.
User button use GPIOA0.

So we just have to enable GPIOA and set it to input mode then check the IDR register to see if we got an input or not.

User button not pushed => input data register PA0 == 0
User button pushed => input data register PA0 == 1

With previous project on LEDS you should be able to do this.
If you have troublem check the code :).

