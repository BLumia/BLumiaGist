import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.Stage;

public class JavaFxTest extends Application {
	@Override
	public void start(Stage primaryStage) {
		Button btn = new Button();
		Text message = new Text();

		btn.setText("Say 'Fuck off!'");

		btn.setOnAction(event -> {
			message.setText("Hello World! JavaFX fucks swing!");
		});

		VBox root = new VBox(10, btn, message);
		root.setAlignment(Pos.CENTER);

		Scene scene = new Scene(root, 300, 250);

		primaryStage.setTitle("MulticastSocketTest");
		primaryStage.setScene(scene);
		primaryStage.show();
	}

	public static void main(String[] args) {
		launch(args);
	}
}
