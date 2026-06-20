# How to PTC rebuild wth different Cload values

1. In case you need to change the values ​​of the CLoud table containing temperature-dependent frequency errors [ppm], you should edit file: `gpAppUcSubsystem_CalibrationDefaultsForEVB.h`
and correct the ```calRamXo32mData_EVB``` table
2. Note that subsequent ```cload_comp``` entries refer to the temperature ranges defined in the table below:

| Parameter     | Temperature range | Frequency Error [ppm] |
|---------------| ----------------- |-----------------------|
| cload_comp_0  | T < -30°C         | 15                    |
| cload_comp_1  | -30°C < T < -20°C | 15                    |
| cload_comp_2  | -20°C < T < -10°C | 15                    |
| cload_comp_3  | -10°C < T < 0°C   | 15                    |
| cload_comp_4  | 0°C < T < 10°C    | 0                     |
| cload_comp_5  | 10°C < T < 20°C   | 0                     |
| cload_comp_6  | 20°C < T < 30°C   | 0                     |
| cload_comp_7  | 30°C < T < 40°C   | 0                     |
| cload_comp_8  | 40°C < T < 50°C   | -15                   |
| cload_comp_9  | 50°C < T < 60°C   | -15                   |
| cload_comp_10 | 60°C < T < 70°C   | -20                   |
| cload_comp_11 | 70°C < T < 80°C   | -20                   |
| cload_comp_12 | 80°C < T < 90°C   | -15                   |
| cload_comp_13 | 90°C < T < 100°C  | -15                   |
| cload_comp_14 | 100°C < T < 110°C | -5                    |
| cload_comp_15 | 110°C < T < 120°C | 10                    |
| cload_comp_16 | 130°C < T         | 25                    |

3. Recompiling PTC code:

- In the next step, you need to rebuild the test application and upload it to the device.

  ```console
  cd Applications/PTC
  make -f Makefile.PTC_QPG6200_10DBM_UART8to9 clean
  make -f Makefile.PTC_QPG6200_10DBM_UART8to9
  ```

- Now you can run the RCC Application to verify the changed values ​​of the calRamXo32mData_EVB table, use the RCC command:
```CLOAD_TABLE```
