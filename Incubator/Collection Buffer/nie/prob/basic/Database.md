1. Disable Pig's MultiQuery Optimization when using DynamoDBStorage

2. DynamoDB's data model does not allow item fields to have null or empty string values. If DynamoDBStorage sees an empty or null value, it will omit that field for the given record.