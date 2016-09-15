from __future__ import print_function
import boto3
from datetime import datetime

def lambda_handler(event, context):
    cwc=boto3.client('cloudwatch', region_name='us-west-2')
    for record in event['Records']:
        sensor_name = record['dynamodb']['NewImage']['payload']['M']['name']['S']
        event_value = record['dynamodb']['NewImage']['payload']['M']['value']['N']
        event_value = float(event_value)
        event_datetime = record['dynamodb']['NewImage']['payload']['M']['timestamp']['S']
        #print(sensor_name)
        #print(event_value)
        #print(event_datetime)
        metricData=[{
            'MetricName': sensor_name,
            'Value': event_value,
            'Timestamp': datetime.strptime(event_datetime, '%Y-%m-%d %H:%M:%S.%f'),
            'Unit': 'Seconds'
        },]
        cwc.put_metric_data(Namespace="PocEventCounterHour",MetricData=metricData)