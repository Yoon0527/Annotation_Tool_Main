## Annotation Tool
- 목적: 내시경 이미지에 포함된 병변의 영역을 표시하기 위한 프로그램
- 개발 기간: 2024.04.18 ~ 2024.04.30 (이후 사용자 피드백에 따라 기능 수정 및 추가 중)

### 설명
1. 로그인
- 사용자의 이름, 소속, 경력을 입력 후 로그인.
![mainwindow](https://github.com/Yoon0527/Annotation_Tool_Main/assets/67099001/d495d72e-5e3f-4b01-8a79-99b8255ffdbf)

2. 메인 창<br/>
![mainwindow](https://github.com/Yoon0527/Annotation_Tool_Main/assets/67099001/11ff5098-e38c-4db4-8465-6de6df1c853d)

a. image load<br/>
- 라벨링 하고자 하는 이미지를 불러온다.
  ![Annotation_Tool_load](https://github.com/Yoon0527/Annotation_Tool_Main/assets/67099001/0a453ea6-2d68-44d3-921f-fb61f3fad987)
<br/><br/>
- 라벨링 하고자 하는 영역을 마우스로 드래그하여 라벨을 표시한다.
![Annotation_Tool_draw](https://github.com/Yoon0527/Annotation_Tool_Main/assets/67099001/03cfd3b0-60ac-444f-8588-c71cddb287e6)
<br/><br/>
- 오른쪽 리스트에 추가되는 라벨을 클릭하면 해당 라벨의 좌표 정보 등을 확인할 수 있다.
![Annotation_Tool_checklabel](https://github.com/Yoon0527/Annotation_Tool_Main/assets/67099001/6f38a881-2ff8-41b0-b0a1-b4dc8ee8290c)
<br/><br/>
b. delete
- 라벨을 삭제하는 기능
- 삭제하고자 하는 라벨을 리스트에서 클릭한 후, delete 버튼을 클릭하여 삭제한다.
![Annotation_Tool_delete](https://github.com/Yoon0527/Annotation_Tool_Main/assets/67099001/6dc9d74e-ba28-4625-9bb9-e011435b2234)
<br/><br/>
c. save
- 사용자가 그린 라벨 정보를 csv로 저장하고, 이미지 중 라벨을 그린 이미지와 그리지 않은 이미지를 분류하는 기능.
![Annotation_Tool_save](https://github.com/Yoon0527/Annotation_Tool_Main/assets/67099001/f4df769d-a4a8-4bef-8dc0-fdd2792a0fd6)
<br/><br/>
d. 그 외
- 이미지 밝기 조절 기능
![Annotation_Tool_brightness](https://github.com/Yoon0527/Annotation_Tool_Main/assets/67099001/28ab7218-8a56-4590-870c-221e10f6369a)

### 이후 수정 사항(사용자 피드백 반영)
- 라벨을 그린 후 바로 csv로 저장되는 기능 추가
- 라벨이 반영된 이미지가 바로 저장되는 기능 추가: 라벨을 그릴 때 딜레이가 발생하여 멀티 스레드로 저장 기능을 추가.

