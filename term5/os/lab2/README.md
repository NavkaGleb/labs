# Operating Systems Lab2

Implemented scheduling algorithm: Fair-Share Algorithm

To run the simulator, you need to pass parameters:

1. Path to config.json file
2. Path to summary_results.txt file
3. Path to process_results.txt file

Example of config.json file

~~~json
{
  "simulation_time": 5000,
  "process_config": [
    {
      "name": "a",
      "user_id": 0,
      "cpu_time_range": {
        "min": 300,
        "max": 600
      },
      "io_block_range": {
        "min": 80,
        "max": 140
      }
    },
    {
      "name": "b",
      "user_id": 1,
      "cpu_time_range": {
        "min": 200,
        "max": 800
      }
    },
    {
      "name": "c",
      "user_id": 1,
      "cpu_time_range": {
        "min": 300,
        "max": 400
      },
      "io_block_range": {
        "min": 30,
        "max": 100
      }
    }
  ]
}
~~~