/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 */

import React, {useRef, useState} from 'react';
import {
  Button,
  SafeAreaView,
  NativeModules,
  Image,
  View,
  ActivityIndicator,
} from 'react-native';
import {
  Camera,
  useCameraDevices,
  CameraPermissionStatus,
} from 'react-native-vision-camera';
const {ImageProcessor} = NativeModules;

function App(): JSX.Element {
  const [imageBase64, setImageBase64] = useState<string>('');
  const [cameraOpen, setCameraOpen] = useState<boolean>(false);
  const devices = useCameraDevices();
  const device = devices.front;

  const camera = useRef<Camera>(null);
  return (
    <SafeAreaView>
      {cameraOpen === true ? (
        device == null ? (
          <ActivityIndicator size={'large'} />
        ) : (
          <View style={{backgroundColor: 'red', padding: 12, height: '100%'}}>
            <Camera
              style={{
                flex: 1,
              }}
              ref={camera}
              device={device}
              isActive={true}
              photo={true}
            />
            <Button
              title="Take Photo"
              onPress={async () => {
                if (camera.current !== null) {
                  const photo = await camera.current.takePhoto({
                    flash: 'on',
                  });
                  setImageBase64(`file://${photo.path}`);
                }
                setCameraOpen(false);
              }}
            />
          </View>
        )
      ) : (
        <View>
          <Button
            title="Take Picture"
            onPress={async () => {
              const cameraPermission = await Camera.getCameraPermissionStatus();

              if (cameraPermission === 'authorized') {
                setCameraOpen(true);
              } else {
                const newCameraPermission =
                  await Camera.requestCameraPermission();
                console.log('New Camera Permission: ', newCameraPermission);
                if (newCameraPermission === 'authorized') {
                  setCameraOpen(true);
                }
              }
              // const processedImageBase64 = await ImageProcessor.processImage();
              // setImageBase64(processedImageBase64);
            }}
          />
          {imageBase64 && (
            <Image source={{uri: `${imageBase64}`}} width={300} height={300} />
          )}
        </View>
      )}
    </SafeAreaView>
  );
}

export default App;
