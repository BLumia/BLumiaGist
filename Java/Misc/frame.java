import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.ListSelectionModel;
import javax.swing.AbstractListModel;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;
import java.awt.event.ActionEvent;

public class frame extends JFrame
{
    public static void main(String[] args)
    {
        new frame().setVisible(true);
    }

    public frame()
    {
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setBounds(100, 100, 450, 300);
        getContentPane().setLayout(null);
        
        List<String> content = new ArrayList<String>();

        JScrollPane scrollPane = new JScrollPane();
        scrollPane.setBounds(10, 10, 414, 208);
        getContentPane().add(scrollPane);

        JList<String> list = new JList<String>();
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        scrollPane.setViewportView(list);

        JTextField textField = new JTextField();
        textField.setBounds(10, 228, 311, 23);
        getContentPane().add(textField);
        textField.setColumns(10);

        JButton btnNewButton = new JButton("Go!");
        btnNewButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent arg0)
            {
                String text = textField.getText();
                if (!text.isEmpty())
                {
                    content.add(text);
                    String[] str = new String[content.size()];
                    content.toArray(str);
                    list.setListData(str);
                    textField.setText("");
                }
            }
        });
        btnNewButton.setBounds(331, 228, 93, 23);
        getContentPane().add(btnNewButton);
    }
}
