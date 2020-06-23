#Testing

As you can see the plugin is bundled with selenium testing on this repository. You can use the tests, if you have some experience with testing it could be helpful. 
*DO NOT USE IN PRODUCTION, THE TESTS MODIFY SETTINGS AND CREATE ORDERS*

## Requirements

* A Drupal Commerce installation is required on Drupal 7
* You also need to have an admin account for which you set the credentials in the .env file
* Lastly, make sure the currencies tested with the plugin are enabled in the admin

## Getting started

1. Follow 1 and 2 from the [Steward readme page](https://github.com/lmc-eu/steward#getting-started)
2. Create an env file in the root folder and add the following:
`
ENVIRONMENT_URL="https://drupalcommerce.url"
ENVIRONMENT_USER="username"
ENVIRONMENT_PASS="yourpassword"
ADMIN_PREFIX="admin"
`

3. Start the testing server. See
[Steward readme page](https://github.com/lmc-eu/steward#4-run-your-tests)
4. Run  ./vendor/bin/steward run staging chrome --group="drupalcommerce_quick_test" -vv for the short test
5. Run  ./vendor/bin/steward run staging chrome -vv to go through all the available tests.

## Problems

Since this is a frontend test, its not always consistent, due to delays or some glitches regarding overlapping elements. If you can't get over an issue please open an issue and I'll take a look. 